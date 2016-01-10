#!/usr/bin/env python

import time
import argparse
import requests
import pyscreenshot

HOST = 'http://127.0.0.1:5000/upload'
SCREENS = 'screens/'


def upload(image):
    image = {'image': open(image, 'r')}
    r = requests.post(HOST, files=image)
    if r.status_code == 200:
        print r.text
    else:
        print "Unknown error!"


def get_screenshot():
    filename = SCREENS + time.strftime('%d_%m_%Y-%H_%M_%S') + ".png"
    pyscreenshot.grab_to_file(filename)
    return filename


def main():
    parser = argparse.ArgumentParser(description="Screen.me upload client.")
    parser.add_argument("image", metavar="I", type=str, nargs="?",
                        help="Image to be uploaded.")
    args = parser.parse_args()

    if args.image:
        upload(args.image)
    else:
        upload(get_screenshot())

if __name__ == "__main__":
    main()
