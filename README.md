Inotify Covert Channel
======================

Simple proof of concept showing inotify as a covert channel. I got this idea
while reading the Page Cache Attacks paper (https://t.co/kkLn23P9GY).
The Covert Channel section in that paper describes using an information leak
in mincore to create a covert channel between two controlled processes that
both have read-only access to a single file.

This proof of concept demonstrates a similar effect by using inotify. The
sender process sends bits by opening, closing and reading a file. To send
a zero bit the file is simply opened and then closed. To send a one bit
the file is opened, a read is performed, and then then the file is closed.

The receiver registers an inotify watch on the read-only file. Whenever an
access event is received the current bit in the stream is set. Whenever a
close event is received the bit position is advanced. Bits in the stream
default to zero.

Building
--------

To build the sender and receiver:

  gcc sender.c -o sender
  gcc receiver.c -o receiver

Running
-------

To run, pick a seldom used file that an unprivileged process can open
read-only. /etc/passwd works well. Start the receiver in one terminal:

  ./receiver /etc/passwd

Then run the sender in a separate terminal with a message to send:

  ./sender /etc/passwd "Hello, World!"

You should see the following in the receiver terminal:

  Byte: H
  Byte: e
  Byte: l
  Byte: l
  Byte: o
  Byte: ,
  Byte:
  Byte: W
  Byte: o
  Byte: r
  Byte: l
  Byte: d
  Byte: !

Impact
------

This is really just a fun proof of concept showing how two processes
can create a communication channel when system calls provide a
capability to read events and aribtrarily trigger those events.
