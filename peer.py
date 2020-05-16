# peers
import getopt,sys
import time
import zmq
from multiprocessing import Process,Value
import requests
import collections
from random import seed
from random import randint
from random import choice
import string
import json
import math
from Crypto.Hash import SHA3_256
from Crypto.PublicKey import ECC
from Crypto.Signature import DSS
from threading import Thread
import os


PEER_COUNT  = Value('i',-1) # N VALUE
HASH_COUNT  = Value('i',-1) # T VALUE
ROUND_COUNT = Value('i',-1) # R VALUE
TRANS_COUNT = Value('i',-1) # L VALUE
SCENARIO_COUNT = Value('i',-1) #S VALUE
K_COUNT = Value('i',-1) #Fault Tolerance
PROPOSER_MALICIOUS_COUNT = Value('i',0) #IS PROPOSER MALICIOUS? 0-NO 1-YES
VALIDATOR_MALICIOUS_COUNT = Value('i',-1) #NUMBER OF MALICIOUS VALIDATORS
VALMAL_COUNT = Value('i',-1) #CONSTANT MAL_VALIDATOR COUNT


URL = "http://127.0.0.1:5000/server"
URLSECRET = "http://127.0.0.1:5000/server/secret"

seed(time.time()) # SEEDING THE RANDOM NUMBER GENERATOR

NUMS = []


def Listener(url,signatureList,peerList,privateKey,givenBlock,round,pid,h_prev,isProposer=False,isMalicious=0): # LISTEN TO OWN ID FOR SIGNED BLOCKS


    context = zmq.Context()
    socket = context.socket(zmq.PULL)
    socket.bind(url)
    messageCount = 0

    #h_prev = SHA3_256.new("".encode('utf-8'))
    
    lenSignatureList = len(signatureList)
    
    blockCounter = {}
    while messageCount < int((PEER_COUNT.value - lenSignatureList - isMalicious)):
        message = socket.recv_json()
        messageCount = messageCount+1
        senderID  = message['pid']
        block     = "".join(str(message['block']))
        signature = int(message['signature']).to_bytes(64,byteorder='big')
        senderKey = ECC.import_key(findKey(peerList,senderID))

        verifier = DSS.new(senderKey,'fips-186-3') # Verifier of proposer
        h = SHA3_256.new(block.encode('utf-8')+ h_prev.digest())

        try:
            verifier.verify(h,signature)
            if block not in blockCounter.keys():
                if givenBlock == block:
                    blockCounter[block] = [1]
                    for signature1 in signatureList:
                        blockCounter[block].append(signature1)
                        if int(pid) != int(signature1['pid']):
                            blockCounter[block][0] = blockCounter[block][0] + 1 
                else:
                    blockCounter[block] = [1]    
            else:
                blockCounter[block][0] = blockCounter[block][0] + 1
            blockCounter[block].append({'pid' : senderID,'signature' : str(int.from_bytes(signature,"big"))})
            
        except ValueError:
            pass
    
                    
    
    path = "Sc" + str(SCENARIO_COUNT.value) + "_Peer_" + str(pid)
    if not os.path.exists(path):
        os.makedirs(path)
    if isMalicious == 0: # if not malicious write, else do not write log
        blockIndex = 0
        for key in blockCounter.keys(): # iterate over recieved blocks
            filename = "block_" + round
            filename = filename + "_" + str(blockIndex) + ".log"
            f = open(os.path.join(path,filename),"wt")
            f.write(key)
            f.write(json.dumps(blockCounter[key][1:]))
            f.close()
            blockIndex = blockIndex + 1
    
    
    socket.unbind(url)

def Collector(url):
    context = zmq.Context()
    socket  = context.socket(zmq.PULL)
    socket.bind(url)

    #print("CONNECTED TO URL " + url)

    for _ in range(PEER_COUNT.value-1):
        message = socket.recv_json()
        #print("MESSAGE RECIEVED!")
        num = message['num']
        NUMS.append(num)
        #print(num)

def help():
    print("*******************************************************************************")
    print("Proper usage of the program is:")
    print("python peer.py -n <peer count> -t <hash count>")
    print("Both -n and -t flags are required flags, and both require an INTEGER arguement.")
    print("Phase 2 update, -r <round count> -l <transaction count>")
    print("Phase 3 update, -s <scenario count>")
    print("*******************************************************************************")

def succ(peerList,pid):
    minID = (2**24)
    minimumElement = (2**24)
    for peer in peerList:
        currID = peer['id']
        if currID >=pid and currID <=minID:
            minID=currID
        if currID <= minimumElement:
            minimumElement = currID
    if minID == (2**24):
        return minimumElement
    return minID



def Send(m,peerList,pid,malMessage=False,malPeers = False): # SENDER OF PROPOSER PROCESS
    
    if malMessage == False or malMessage == {}:
        for peer in peerList:
            context = zmq.Context()
            if peer['id'] != pid:
                socket = context.socket(zmq.PUSH)  ###########  CREATE SOCKETS INSIDE LOOP
                socket.connect("tcp://127.0.0.1:"+ str(peer['port']))
                socket.send_json(m)
                #print("Message sent to " +  "tcp://127.0.0.1:"+str(peer['port']))
    else:
        trueCount = 0
        for peer in peerList:
            context = zmq.Context()
            if peer['id'] != pid:
                socket = context.socket(zmq.PUSH)  ###########  CREATE SOCKETS INSIDE LOOP
                socket.connect("tcp://127.0.0.1:"+ str(peer['port']))
                honestNodes = math.ceil((PEER_COUNT.value - VALMAL_COUNT.value - 1) / 2)
                if (peer['id'] not in malPeers) and trueCount < honestNodes:
                    socket.send_json(m)
                    trueCount = trueCount + 1
                elif peer['id'] in malPeers:
                    socket.send_json(m)        
                else:
                    socket.send_json(malMessage)
                        

def Propose(r,l,pid,key,peerList,port,isMalicious):
    time.sleep(5)
    signer = DSS.new(key,'fips-186-3')
    h_prev = SHA3_256.new("".encode('utf-8'))
    for round in range(0,r):
        block = ""
        for transaction in range(l):
            tau = "".join([choice(string.ascii_letters + string.digits) for n in range(64) ])
            block += (tau + "\n")
        h = SHA3_256.new(block.encode('utf-8') + h_prev.digest())
        malPeers = []
        malToVerifiers = {}
        if (ROUND_COUNT.value - 1 != int(round)) or (isMalicious == 0):
            signature = signer.sign(h)
        else:
            signature = signer.sign(h)
            response = requests.get(URLSECRET)
            maldata = response.json()
            malblock = maldata[0]['block']
            malh = SHA3_256.new(malblock.encode('utf-8')+ h_prev.digest())
            malsignature = signer.sign(malh)
            malToVerifiers = {'block' : malblock, 'signature' : str(int.from_bytes(malsignature,"big")),'pid' : pid} #Malicious message to send to verifiers
            malsignatures = [{'pid' : pid, 'signature' : str(int.from_bytes(malsignature,"big"))}]
            for mal in maldata:
                malPeers.append(mal['id'])
            
        #h_prev = h

        signatures = [{'pid' : pid, 'signature' : str(int.from_bytes(signature,"big"))}]
        
        	
        listener_thread = Thread(target=Listener,args=("tcp://127.0.0.1:" + str(port),signatures,peerList,key,block,str(round),str(pid),h_prev, True))
        listener_thread.start()
        time.sleep(2) 

        toVerifiers = {'block' : block, 'signature' : str(int.from_bytes(signature,"big")),'pid' : pid} # Message to send to verifiers
        if (ROUND_COUNT.value - 1 == int(round)) and isMalicious == 1:
            Send(toVerifiers,peerList,pid,malToVerifiers,malPeers)
        else:
            Send(toVerifiers,peerList,pid)
        listener_thread.join()
        h_prev = h
        time.sleep(2)


def findKey(peerList,pid):
    for peer in peerList:
        if peer['id'] == pid:
            return peer['key']

def Verify(url,pid,peerList,privateKey,isMalicious):
    


    h_prev = SHA3_256.new("".encode('utf-8'))

    for round in range(0,ROUND_COUNT.value):
        context = zmq.Context()
        socket = context.socket(zmq.PULL)
        socket.bind(url)
        proposerMessage = socket.recv_json() ###################
        

        senderID  = proposerMessage['pid']
        block     = "".join(str(proposerMessage['block']))
        signatureProposer = int(proposerMessage['signature']).to_bytes(64,byteorder='big')
        senderKey = ECC.import_key(findKey(peerList,senderID))
        #print("Round " + str(round) + "Proposer ID : " + str(senderID))

        verifier = DSS.new(senderKey,'fips-186-3') # Verifier of proposer
        signer   = DSS.new(privateKey,'fips-186-3') # signer of validator
        h = SHA3_256.new(block.encode('utf-8')+ h_prev.digest())

        try:
            verifier.verify(h,signatureProposer)
            socket.unbind(url)


            signature = signer.sign(h)
            malPeers = []
            malToVerifiers = {}
            if int(round) == ROUND_COUNT.value - 1 and isMalicious == 1 and PROPOSER_MALICIOUS_COUNT.value == 0:
                response = requests.get(URLSECRET)
                maldata = response.json()
                block = maldata[0]['block']
                h = SHA3_256.new(block.encode('utf-8')+ h_prev.digest())
                signature = signer.sign(h)
           
            elif int(round) == ROUND_COUNT.value - 1 and isMalicious == 1 and PROPOSER_MALICIOUS_COUNT.value == 1:
                response = requests.get(URLSECRET)
                maldata = response.json()
                malblock = maldata[0]['block']

                malh = SHA3_256.new(malblock.encode('utf-8')+ h_prev.digest())
                malsignature = signer.sign(malh)
                malToVerifiers = {'block' : malblock, 'signature' : str(int.from_bytes(malsignature,"big")),'pid' : pid} #Malicious message to send to verifiers
                malsignatures = [{'pid' : pid, 'signature' : str(int.from_bytes(malsignature,"big"))}]
                for mal in maldata:
                    malPeers.append(mal['id'])
            
            signatures = [{'pid' : pid,'signature':str(int.from_bytes(signature,"big"))}]
	    
            malSituation = 1
            if (isMalicious == 1 and int(round) != (ROUND_COUNT.value - 1)) or isMalicious == 0:
                signatures.append({'pid' : senderID,'signature':str(int.from_bytes(signatureProposer,"big"))})
                malSituation = 0

            listener_thread = Thread(target=Listener,args=(url,signatures,peerList,privateKey,block,str(round),str(pid),h_prev,None,malSituation))
            listener_thread.start()
            time.sleep(2)
            
            toValidators = {'block' : block, 'signature' : str(int.from_bytes(signature,"big")),'pid' : pid} # Message to send to verifiers
            if PROPOSER_MALICIOUS_COUNT.value == 1 and isMalicious == 1 and (int(round) == ROUND_COUNT.value - 1):
                Send(toValidators,peerList,pid,malToVerifiers,malPeers)
            else:
                Send(toValidators,peerList,pid)
            listener_thread.join()

        except ValueError:
            pass
        h_prev = h


def Peer():
    while True:
        pid             = randint(0,(2 ** 24)-1)
        pport           = randint(1024,65535)

        privateKey      = ECC.generate(curve='NIST P-256')
        publicKey       = privateKey.public_key()
        pkey            = publicKey.export_key(format= "OpenSSH")
        peerData        = {'id' : pid , 'port' : pport, 'key' : pkey}
        response        = requests.post((URL),json = peerData)

        if response.status_code == 201: # SUCCESS
            break

    time.sleep(5) # GIVING TIME TO NETWORK
    response = requests.get((URL))

    #print("PROCESS ID " + str(pid) + " IS NOW REGISTERED!")

    data = response.json()
    #print(data)

    randomNumber = randint(0,(2**256) - 1)
    context      = zmq.Context()
    NUMS.append(randomNumber)

    ownUrl       = "tcp://127.0.0.1:" + str(pport)

    thread_p = Thread(target=Collector,args=(ownUrl,)) # A THREAD TO COLLECT NUMBERS
    thread_p.start()
    time.sleep(2)

    for pe in data:
        if pe['id'] != pid: # DO NOT SEND MESSAGE TO ITSELF
            sender       = context.socket(zmq.PUSH)
            urlToConnect = "tcp://127.0.0.1:" + str(pe['port'])
            sender.connect(urlToConnect)
            sender.send_json({'num' : randomNumber})
            #print("SENT TO " + urlToConnect)

    thread_p.join()
    toBeHashed = 0
    fname = 'election_' + str(pid) + '.log'
    message = ""
    for number in NUMS:
        toBeHashed = toBeHashed  ^ int(number)
        message = message + (str(number) + "\n")

    digest = SHA3_256.new(toBeHashed.to_bytes(32,byteorder = 'big'))

    for _ in range(HASH_COUNT.value-1):
        digest = SHA3_256.new(digest.digest())

    digest = int.from_bytes(digest.digest(),"big") % (2 ** 24)
    digest = succ(data,digest) # digest has the proposer value

    message = message + (str(digest) + "\n")
    signer = DSS.new(privateKey,"fips-186-3")
    signature = signer.sign(SHA3_256.new(str.encode(message)))
    message = message + str(int.from_bytes(signature,'big')) + "\n"
    message = message + str(pkey)
    f = open(fname,"wt")
    f.write(message)
    f.close()
    ############################################
    if digest != pid: # Validator
        isMalicious = 0
        while VALIDATOR_MALICIOUS_COUNT.value > 0:
            isMalicious = randint(0,1)
            if isMalicious == 1:
                VALIDATOR_MALICIOUS_COUNT.value = VALIDATOR_MALICIOUS_COUNT.value - 1
                break
        if isMalicious == 1:
            peerData['l'] = TRANS_COUNT.value
            response = requests.post((URLSECRET),json = peerData)
            
        Verify("tcp://127.0.0.1:" + str(pport),pid,data,privateKey,isMalicious)

    else: # Proposer
        isMalicious = 0
        pposer = PROPOSER_MALICIOUS_COUNT.value
        while pposer > 0:
            isMalicious = randint(0,1)
            if isMalicious == 1:
                pposer = pposer - 1
                break
        if isMalicious == 1:
            peerData['l'] = TRANS_COUNT.value
            response = requests.post((URLSECRET),json = peerData)
        Propose(ROUND_COUNT.value,TRANS_COUNT.value,pid,privateKey,data,pport,isMalicious)




    #print(pid, len(NUMS))



    #print(response.json())
    #print(pid)

if __name__ == '__main__':

    try:
        opts,args = getopt.getopt(sys.argv[1:],"n:t:r:l:s:")
    except getopt.GetoptError as err:
        print(str(err))
        help()
        sys.exit()

    isGivenPeerCount  = False
    isGivenHashCount  = False
    isGivenRoundCount = False
    isGivenTransCount = False
    isGivenScenarioCount = False

    for o, a in opts:
        if o == '-n':
            try:
                PEER_COUNT.value = int(a)
                isGivenPeerCount = True
            except ValueError:
                print("Please enter an integer for peer count flag!")
                sys.exit()
        elif o == '-t':
            try:
                HASH_COUNT.value = int(a)
                isGivenHashCount = True
            except ValueError:
                print("Please enter an integer for hash count flag!")
                sys.exit()
        elif o == '-r':
            try:
                ROUND_COUNT.value = int(a)
                isGivenRoundCount = True
            except ValueError:
                print("Please enter an integer for round count flag!")
                sys.exit()
        elif o == '-l':
            try:
                TRANS_COUNT.value = int(a)
                isGivenTransCount = True
            except ValueError:
                print("Please enter an integer for transaction count flag!")
                sys.exit()
	
	
        elif o == '-s':
            try:
                SCENARIO_COUNT.value = int(a)
               	if SCENARIO_COUNT.value > 0 or SCENARIO_COUNT.value < 5:
                    isGivenScenarioCount = True
		
                else:
                    print("Please enter an integer in the range [1,4]")
                    sys.exit()
            except ValueError:
                print("Please enter an integer for transaction count flag!")
                sys.exit()
        else:
            help()
            sys.exit()

    if isGivenHashCount == False or isGivenPeerCount == False or isGivenRoundCount == False or isGivenTransCount == False:
        help()
        sys.exit()

    peers = []
    K_COUNT.value = int((PEER_COUNT.value - 1) / 3)


    if SCENARIO_COUNT.value==1:
        VALIDATOR_MALICIOUS_COUNT.value = K_COUNT.value
        VALMAL_COUNT.value = K_COUNT.value

    elif SCENARIO_COUNT.value==2:
        PROPOSER_MALICIOUS_COUNT.value = 1
       	VALIDATOR_MALICIOUS_COUNT.value = K_COUNT.value - 1
        VALMAL_COUNT.value = K_COUNT.value -1

    elif SCENARIO_COUNT.value==3:
        VALIDATOR_MALICIOUS_COUNT.value = K_COUNT.value + 1
        VALMAL_COUNT.value = K_COUNT.value +1

    elif SCENARIO_COUNT.value==4:
        PROPOSER_MALICIOUS_COUNT.value = 1
        VALIDATOR_MALICIOUS_COUNT.value = K_COUNT.value
        VALMAL_COUNT.value = K_COUNT.value
        
        

    #print(PEER_COUNT.value)
    for i in range(PEER_COUNT.value):
        peer = Process(target=Peer)
        peer.start()
        peers.append(peer)



    for p in peers:
        p.join()












