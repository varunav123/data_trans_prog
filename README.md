# ARQ-UDP

Implementation of ARQ on top of UDP sockets using Stop and Wait algorithm.

Run the following commands to execute the code as needed (get delay)

```bash
sudo tc qdisc add dev lo root netem delay <Delay_in_miliseconds>
```
assuming `lo` is your loopback device but you should use
`ip addr` to get yours.

```bash
gcc sender.c -o sender
gcc receiver.c -o receiver
```
```bash
./sender <SenderPort> <ReceiverPort> <RetransmissionTimer> <NoOfPacketsToBeSent>
./receiver <ReceiverPort> <SenderPort> <PacketDropProbability>
```
Ex: `./sender 8080 4040 100 4` and `./receiver 8080 4040 0.6`

Note: `NoOfPacketsToBeSent < 99999` and `RetransmissionTimer > 2*Delay_in_miliseconds`

After execution, two files containing output, `sender.txt` and `receiver.txt` will be produced
