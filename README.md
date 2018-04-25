---  

### Reliable Broadcast

Party

**uses** UDP

*n* - total nodes.
*t* - potential malicious nodes

 - waits for message from client
 - sends **echo{message, nonce}** to other parties  
 - waits for echos:
    - received `(n+t+1)/2` **echo** messages and not sent **ready** -> send **ready{message, nonce}** to other parties
 - waits for readies:
    - received `t+1` **ready** messages and not sent **ready** -> send **ready{message, nonce}** to other parties
    - received `2t+1` **ready** messages -> commit 
 
