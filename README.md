# ARQ-UDP

We are implementing ARQ on top of UDP sockets. We have used Stop and Wait algorithm.

To emulate network delay, on linux machine you can use:

```bash
sudo tc qdisc add dev lo root netem delay <Delay_in_miliseconds>
```
assuming `lo` is your loopback device but you should use
`ip addr` to get yours.

to compile,
```bash
gcc sender.c -o sender
gcc receiver.c -o receiver
```
To run the program,
```bash
./sender <SenderPort> <ReceiverPort> <RetransmissionTimer> <NoOfPacketsToBeSent>
./receiver <ReceiverPort> <SenderPort> <PacketDropProbability>
```
Ex: `./sender 8080 4040 100 4` and `./receiver 8080 4040 0.6`

Note that `NoOfPacketsToBeSent < 99999` and `RetransmissionTimer > 2*Delay_in_miliseconds`

After execution, two files containing output, `sender.txt` and `receiver.txt` will be produced
