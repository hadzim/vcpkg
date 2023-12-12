#! /usr/bin/env python3
# Download additional external files from Dulik FTP server

import os
from ftplib import FTP

def main():
    print('Downloading external files from Dulik FTP server...')
    cwd_to_script(__file__)
    ftp = FtpClient()
    ftp.download_tree('/pub/SDK/vcpkg/', '.')
    print('Download successful. Now run copy-external.bat to copy the files.')

class FtpClient:
    '''Client for Dulik FTP server.'''

    def __init__(self):
        self.ftp = FTP('dulik.dev.tbscz', 'anonymous', '')

    def __exit__(self):
        self.ftp.quit()

    def download_tree(self, remote_path, local_path, show_just_finished = False):
        try:
            self.ftp.cwd(remote_path)
            os.makedirs(local_path, exist_ok=True)
            filenames = self.ftp.nlst()

            for filename in filenames:
                lpath = os.path.join(local_path, filename)
                if "." in filename:
                    print(f'Downloading from FTP: {filename}')
                    self.ftp.voidcmd('TYPE I')
                    tracker = _FtpProgressTracker(self.ftp.size(filename), show_just_finished)
                    file = open(lpath, 'wb')
                    self.ftp.retrbinary("RETR " + filename, lambda *args: self.__writeFile(file, tracker, *args), 8192)
                    file.close()
                    print('') # after progress print new line
                else:
                    self.download_tree(filename, lpath, show_just_finished)
        except Exception as e:
            print("Error:", e)

    def __writeFile(self, file, tracker, block):
        file.write(block)
        tracker.handle(block)

class _FtpProgressTracker:
    sizeWritten = 0
    totalSize = 0
    showJustFinished = False

    def __init__(self, totalSize, showJustFinished = False):
        self.totalSize = totalSize
        self.showJustFinished = showJustFinished

    def handle(self, block):
        self.sizeWritten += len(block)
        percentComplete = int(self.sizeWritten * 100 / self.totalSize)
        if not self.showJustFinished or (self.showJustFinished and percentComplete == 100):
            readableCurrent = readableSize(self.sizeWritten)
            readableTotal = readableSize(self.totalSize)
            fillerSpaces = '                           '
            print(f'Progress: {percentComplete}%  {readableCurrent} / {readableTotal}{fillerSpaces}', end='\r')

def readableSize(size, precision=2):
    suffixes=['B','KB','MB','GB','TB']
    suffixIndex = 0
    while size >= 1024 and suffixIndex < len(suffixes):
        suffixIndex += 1
        size = size/1024.0
    return "%.*f%s"%(precision,size,suffixes[suffixIndex])

def cwd_to_script(file: str):
    # cwd to the script's directory
    script_path = os.path.abspath(os.path.dirname(file))
    os.chdir(script_path)

if __name__ == '__main__':
    main()
