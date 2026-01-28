#!/usr/bin/python3
import serial
import struct
from crc import Calculator, Crc8

class _SerialMsgMeta(type):
    def __init__(cls, name, bases, ns):
        super().__init__(name, bases, ns)
        if name == "SerialMsg":
            cls.ACK_MSG = cls(cls.ACT_META, [cls.ACK_VALUE])
            cls.NAK_MSG = cls(cls.ACT_META, [cls.NAK_VALUE])


class SerialMsg(metaclass=_SerialMsgMeta):
    SYNC = 0x6721
    PAYLOAD_MAX_LEN = 8
    __crc_calc = Calculator(Crc8.CCITT) # if you didnt know, calc is short for calculator

    ACT_META        = 0x00
    ACT_POWER_REQ   = 0x01
    ACT_STATUS_REQ  = 0x02
    ACT_STATUS_RX   = 0x03

    ACK_VALUE       = 0x01
    NAK_VALUE       = 0x00

    MIN_SIZE = 5
    MAX_SIZE = PAYLOAD_MAX_LEN + MIN_SIZE

    def __init__(self, action : int, payload : bytes | list[int], sync : int = SYNC):
        """
        __init__
        
        :param action: Action to perfom, 1 byte
        :type action: int
        :param payload: Bytes array to send, up to 8 bytes
        :type payload: bytes
        :param sync: Optional, Sync (aka start delimiters) bytes, 2 bytes
        :type sync: int
        """
        if not isinstance(sync, int):
            raise TypeError("Sync must be int")
        if not isinstance(action, int):
            raise TypeError("Action must be int")
        
        if not isinstance(payload, (bytes, bytearray, list)):
            raise TypeError("payload must be bytes, bytearray, or list[int]")
        if isinstance(payload, list):
            if not all(isinstance(b, int) and 0 <= b <= 255 for b in payload):
                raise ValueError("payload list elements must be ints in range 0..255")

        self.sync = sync & 0xFFFF
        self.action = action & 0xFF
        self.payload = bytes(payload[:type(self).PAYLOAD_MAX_LEN])

    def convert_msg_to_bytes(self) -> bytes:
        '''
        Method that converts the current message to a byte array. Typically to send it through a serial device.
        
        :return: bytes array of the message
        :rtype: bytes
        '''
        sync_bytes = struct.pack(">H", self.sync & 0xFFFF)
        middle_bytes = struct.pack("<BB", self.action & 0xFF, len(self.payload) & 0xFF) + self.payload

        crc8 = self.get_checksum()
        crc8_byte = struct.pack("<B", crc8 & 0xFF)

        return sync_bytes + middle_bytes + crc8_byte
    
    def get_checksum(self) -> int:
        '''
        Method that calculates the checksum of the current message
        
        :return: Description
        :rtype: int
        '''
        length = len(self.payload)
        data = struct.pack("<BB", self.action & 0xFF, length & 0xFF) + self.payload
        return SerialMsg.__crc_calc.checksum(data)

    def __str__(self):
        payload = ''.join(f'x{byte:02x}' for byte in self.payload)
        return f"Sync = {hex(self.sync)}, Action = {self.action}, Payload length = {len(self.payload)}, Payload = {payload}, CRC8 = {hex(self.get_checksum())}"

    def __eq__(self, value) -> bool:
        if not isinstance(value, SerialMsg):
            return NotImplemented

        return (self.sync == value.sync and self.action == value.action and self.payload == value.payload and self.get_checksum() == value.get_checksum())

class PicoSerial:
    '''
    PicoSerial Class
    Public Variables:
    - ser : Serial object, serial device connected to
    - sent_buf : bytes, buffer of previous message sent
    - received_buf : bytes, buffer of previous message received
    - logging : bool, if True, prints logging messages during Tx or Rx

    Public Functions: 
    - connect: TODO
    - close: closes serial device
    - send_msg: sends a message to serial device
    - read_msg: receives a message from serial device 
    '''

    def __init__(self, port : str, baud : int = 115200, timeout : float = 0.1, logging : bool = False):
        '''
        __init__
        
        :param port: string of device to connect to
        :type port: str
        :param baud: baudrate of serial device, default is 115200
        :type baud: int
        :param timeout: timeout value (in seconds) a serial device sound wait
        :type timeout: float
        :param logging: boolean that enables log messages when sending or receiving
        :type logging: bool
        '''
        self.ser = serial.Serial(port, baud, timeout=timeout)
        self.sent_buf = bytes()
        self.received_buf = bytes()
        self.logging = logging
        pass

    def __del__(self):
        self.ser.close()

    def connect(self):
        '''
        TODO: Docstring for connect
        
        :param self: Description
        '''
        # TODO
        pass 

    def close(self) -> None:
        '''
        Closes the serial device
        '''
        self.ser.close()

    def __stringify(self, bytesarr : bytes | list[int]) -> str:
        return ' '.join(f'x{byte:02x}' for byte in bytesarr)
    
    def send_msg(self, action : int, payload: bytes| list[int]) -> int:
        '''
        Sends a message to the serial device. Will print log message if object's logging is true.
        
        :param action: Action byte of the message
        :type action: int
        :param payload: Payload bytes array or list to send
        :type payload: bytes | list[int]
        :return: Status value if succeeded or not in sending. Any non-zero number means it failed.
        :rtype: int
        :rval:
        '''
        if len(payload) > SerialMsg.PAYLOAD_MAX_LEN:
            return 1
        
        msg = SerialMsg(action, bytes(payload))
        self.sent_buf = msg.convert_msg_to_bytes()

        if self.logging:
            print(f"Sending: {self.__stringify(self.sent_buf)}")

        self.ser.write(self.sent_buf)

        return 0

    def read_msg(self, as_hex : bool = False) -> list[int] | str:
        '''
        Reads from the serial device. Will print log message if object's logging is true.
        
        :param as_hex: Boolean for whether or not, the return type is a int list or the hex string version of it.
        :type as_hex: bool
        :return: Either a int list or hex string list of the read buffer
        :rtype: list[int] | str
        '''
        self.received_buf = self.ser.read_until()
        received_list = list(struct.unpack(f'{len(self.received_buf)}B', self.received_buf))

        if as_hex:
            result = self.__stringify(received_list)
        else:
            result = received_list

        if self.logging:
            print(f"Received: {result}")
        return result
    
    def parse_received_buf(self) -> SerialMsg | None:
        buf = self.received_buf
        buf_size = len(buf)

        if buf_size < SerialMsg.MIN_SIZE:
            if self.logging:
                print("Size of buffer is too small to be a message.")
            return None
                
        sync = (buf[0] << 8) | buf[1]
        action = buf[2]
        payload_len = buf[3]

        expected_len = 4 + payload_len + 1
        if buf_size != expected_len:
            if self.logging:
                print(f"Size of message is not the expected length. Received = {buf_size}, expected = {expected_len}.")
            return None
        
        payload = buf[4 : 4 + payload_len]
        crc8 = buf[4 + payload_len]

        msg = SerialMsg(action, payload, sync=sync)

        if self.logging:
            print(msg)

        if crc8 != msg.get_checksum():
            if self.logging:
                print("Received CRC does not match calculated CRC")
                print(f"Given = {crc8}, Calculated = {msg.get_checksum()}")
            return None
        
        return msg        


if __name__ == "__main__":
    PORT = "/dev/ttyACM0"

    device = None
    try: 
        device = PicoSerial(PORT, logging=True)

        stars = ("**********************************")
        print(stars)
        print("Testing Serial Power Motor Drivers")
        print(stars)

        while True:

            action = input("\nEnter action code: ")

            while not action.isdigit():
                action = input("Invalid, Enter action code: ")

            action = int(action)

            payload = []

            while True:
                if len(payload) >= SerialMsg.PAYLOAD_MAX_LEN:
                    print(f"Max payload size ({SerialMsg.PAYLOAD_MAX_LEN}) reached, ")
                    break

                val = input("Enter payload, blank to end: ").strip()
                if val == "":
                    break
                if not val.isdigit():
                    print(f"{val} is invalid.", end=" ")
                    continue

                payload.append(int(val))

            print(f"action = {action}, payload = {payload}, length = {len(payload)}")

            device.send_msg(action, payload)

            output = device.read_msg(True)        
            received_msg = device.parse_received_buf()

            if (received_msg == SerialMsg.ACK_MSG):
                print("Recieved an ACK")
            elif received_msg == SerialMsg.NAK_MSG:
                print("Received a NAK")
            elif received_msg is not None:
                print("Some other type of message was received")
            else:
                print("There was an error when parsing the message")

    except KeyboardInterrupt:
        print("\nKeyboard interupt has occured")
    finally:
        print(stars)
        print("Program is ending...")
        print(stars)

        if device is not None:
            device.close()
            print("Device closed")