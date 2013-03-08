#!/usr/bin/env python

import socket
import datetime
import time
import json

TCP_IP = '127.0.0.1'
TCP_PORT = 8081
BUFFER_SIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))


f = open('/home/chrisschaefer/Dokumente/typology/test.file', 'r')
o = open('test.file.out', 'w')
lineNr = 0
for line in f:
  lineNr += 1
  word = line.split(' ', 5)
  try:
    maxChar = min(6, len(word[4]) + 1)
    for i in range(maxChar):
      message = word[3] + ' ' + word[4][0:i]
      now = datetime.datetime.now()
      start = time.mktime(now.timetuple())*1e3 + now.microsecond/1e3
      s.send(message)
      data = s.recv(BUFFER_SIZE)
      ##print data
      now = datetime.datetime.now()
      stop = time.mktime(now.timetuple())*1e3 + now.microsecond/1e3
      jsonObj = json.loads(data)
      ##print json.dumps(jsonObj)
      o.write(str(lineNr) + ' ' + str(i) + ' ' + str(stop-start) + ' ' + jsonObj['time_in_ms'] + ' ' + word[3] + ' ' + word[4] + ' ' + jsonObj['results'][0] + '\n')
  except:
    continue
s.close()

##print "received data:", data
