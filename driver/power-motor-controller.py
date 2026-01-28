from serial_message_driver import PicoSerial
import sys

FLAGS = ['-s', "--status", "-p", "--power", "-d", "--device"]

def main(): 
    argc = len(sys.argv)
    argv = sys.argv

    help_msg = f'''{argv[0]}: Program that connects to  
            '''

    if argc == 1: 
        print(f"{argv[0]} needs some paramets")
        return 1
    else:
        print(f"argc = {argc}")
        print(f"argv = {argv}")

    return 0


if __name__ == "__main__":
    main()