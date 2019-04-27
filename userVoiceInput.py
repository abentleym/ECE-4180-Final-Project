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


def get_hints(language_code):
    if language_code.startswith('en_'):
        return ('turn on the light',
                'turn off the light',
                'blink the light',
                'goodbye',
                'know',
                'turn',
                'find',
                'repeat')
    return None

def locale_language():
    language, _ = locale.getdefaultlocale()
    return language

def main():
    
    logging.basicConfig(level=logging.DEBUG)

    parser = argparse.ArgumentParser(description='Assistant service example.')
    parser.add_argument('--language', default=locale_language())
    args = parser.parse_args()

    logging.info('Initializing for language %s...', args.language)
    hints = get_hints(args.language)
    client = CloudSpeechClient()
    with Board() as board:
        while True:
            if hints:
                logging.info('Say something, e.g. %s.' % ', '.join(hints))
            else:
                logging.info('Say something.')
            text = client.recognize(language_code=args.language,
                                    hint_phrases=hints)
            if text is None:
                logging.info('You said nothing.')
                continue

            logging.info('You said: "%s"' % text)
            text = text.lower()
            if 'do you know this pattern' in text:
                writeFile("voiceCommand.txt" , "know")
                break
            elif 'find' in text:
                writeFile("voiceCommand.txt", text)
                break
            elif 'turn' in text:
                writeFile("voiceCommand.txt","turn")
                break
            elif 'goodbye' in text:
                break

if __name__ == '__main__':
    main()
