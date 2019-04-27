#! /usr/bin/env python3
"""A demo of the Google CloudSpeech recognizer."""
import argparse
import locale
import logging

from aiy.board import Board, Led
from aiy.cloudspeech import CloudSpeechClient

import aiy.voice.tts


def readFile(filename):
    f = open(filename, "r")
    if f.mode == "r":
        contents = f.read()
        return contents
    

def writeFile(filename, cmdTOCPP):
    f = open(filename, "w")
    f.write(cmdTOCPP)
    f.close()


def locale_language():
    language, _ = locale.getdefaultlocale()
    return language

def main():
    
    logging.basicConfig(level=logging.DEBUG)

    parser = argparse.ArgumentParser(description='Assistant service example.')
    parser.add_argument('--language', default=locale_language())
    args = parser.parse_args()

    logging.info('Initializing for language %s...', args.language)
    
    client = CloudSpeechClient()
    with Board() as board:
        aiy.voice.tts.say(readFile("pixyResponse.txt"),'en-GB', 10,180, 90, 'default' ) #text,language,volume,pitch,speed,device='default'


if __name__ == '__main__':
    main()
