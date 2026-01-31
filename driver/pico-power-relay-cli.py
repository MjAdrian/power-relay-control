#!/usr/bin/python3
from serial_message_driver import PicoSerial, SerialMsg
from serial import SerialException
import argparse

def buildPowerRelayParser() -> argparse.ArgumentParser:
    '''
    Builds the Parser for the Power Relay CLI
    
    :return: Argument Parser object
    :rtype: ArgumentParser
    '''

    p = argparse.ArgumentParser(description="Connects to a Pico and sends serial command messages.")

    power_flags = ("--power", "-p")
    power_choices = ("on", "off", "1", "0")

    status_flags = ("--status", "-s")
    port_flags = ("--port", "-P", "--device", "-d")
    baud_flags = ("--baud", "-b")
    timeout_flags = ("--timeout" , "-t")
    verbose_flags = ("--verbose", "-v")
    
    p.add_argument(
                   *power_flags,    
                   choices=power_choices,  
                   help="Set state of the Power Relay Control Module."
                   )
    
    p.add_argument(
                   *status_flags,
                   action='count',
                   default=0,
                   help="Request status of the Power Relay Control Module.Repeat to force status before and after other actions."
                   )

    p.add_argument(
                   *port_flags,
                   default="/dev/ttyACM0",
                   help="Serial Device Port (default is /dev/ttyACM0)."
                   )
    p.add_argument(
                   *baud_flags,
                   type=int,
                   default=115200, 
                   help="Baud rate (default: 115200)." 
                   )
    p.add_argument(*timeout_flags,
                   type=int,
                   help="Timeout time in seconds (default:0.1s)."
                   )

    p.add_argument(
                   *verbose_flags,
                   action='store_true',
                   help="Print logging messages"
                   )

    return p


def sendPower(dev: PicoSerial, args: argparse.Namespace) -> int:
    '''
    Sends a power message to serial device to turn it on or off.
    
    :param dev: Pico Serial Device Object 
    :type dev: PicoSerial
    :param args: arguments from argument parser
    :type args: argparse.Namespace
    :return: Error stauts, 0 = success, >0: failed
    :rtype: int
    '''
    if dev is None:
        return 1
    
    if args.verbose:
        print("Sending Power Request Message...")

    if args.power == "on" or args.power == "1":
        if args.verbose:
            print("Turning on power...")
        dev.send_msg(SerialMsg.ACT_POWER_REQ, [1])
    elif args.power == "off" or args.power == "0":
        if args.verbose:
            print("Turning off power...")
        dev.send_msg(SerialMsg.ACT_POWER_REQ, [0])
    else:
        if args.verbose:
            print("How???")
        pass

    receieved_msg = dev.read_msg()

    if args.verbose:
        msg_type = "ACK message" if receieved_msg == SerialMsg.ACK_MSG else "NAK message" if receieved_msg == SerialMsg.NAK_MSG else receieved_msg
        print(f"Received: {msg_type}") # i am so sorry

    return 0


def sendStatus(dev: PicoSerial, args: argparse.Namespace) -> int:
    '''
    Sends a status message to serial device to check current state of device

    :param dev: Pico Serial Device Object 
    :type dev: PicoSerial
    :param args: arguments from argument parser
    :type args: argparse.Namespace
    :return: Error stauts, 0 = success, >0: failed
    :rtype: int
    '''
    if dev is None:
        return 1
    
    if args.verbose:
        print("Sending a status message")

    dev.send_msg(SerialMsg.ACT_STATUS_REQ, [])

    receieved_msg = dev.read_msg()

    if args.verbose:
        print(f'Recevied: {receieved_msg}')

    return 0

def main():
    parser = buildPowerRelayParser()
    args = parser.parse_args()

    dev = None
    try: 
        dev = PicoSerial(port=args.port, baud=args.baud, timeout=args.timeout)
    except SerialException as e:
        print(f"Serial Device error:\n{e}")
        return 1

    err_count = 0
    if args.status > 1 and args.power:
        err_count += sendStatus(dev, args)
    if args.power:
        err_count += sendPower(dev, args)
    if args.status:
        err_count += sendStatus(dev, args)

    # remeber true = 1 and false = 0, so if err == 0, then it will be
    return not (err_count == 0)

if __name__ == "__main__":
    raise SystemExit(main())