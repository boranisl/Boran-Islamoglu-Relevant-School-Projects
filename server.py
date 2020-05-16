# index server
from flask import Flask, request,jsonify,make_response,Response
from flask_restful import Resource, Api
from random import choice
import string

app = Flask(__name__)
api = Api(app)

peerList = []
peerListForTest = {}
maliciousPeers = []
block = ""


class Server(Resource):
    def post(self):
        data     = request.get_json()
        peerID   = data['id']
        peerPort = data['port']
        peerKey  = data['key']

        for peer in peerList:
            if peer['id'] == peerID or peer['port'] == peerPort:
                return Response(status = 404)
        
        peerDict = {'id' : peerID, 'port' : peerPort, 'key' : peerKey}
        peerList.append(peerDict)

        peerListForTest[peerID] = peerKey
        return Response(status=201)

    def get(self):
        response = jsonify(peerList)
        return response

class ServerTest(Resource):
    def get(self):
        response = jsonify(peerListForTest)
        return response

class ServerSecret(Resource):
    def get(self):
        response = jsonify(maliciousPeers)
        return response
    def post(self):
        global block
        data     = request.get_json()
        peerID   = data['id']
        peerPort = data['port']
        peerKey  = data['key']
        if block == "":
            for transaction in range(data['l']):
                tau = "".join([choice(string.ascii_letters + string.digits) for n in range(64) ])
                block += (tau + "\n")
        peerDict = {'id' : peerID, 'port' : peerPort, 'key' : peerKey, 'block': block}
        maliciousPeers.append(peerDict)
        return Response(status=201)

api.add_resource(Server,"/server")
api.add_resource(ServerTest,"/server/test")
api.add_resource(ServerSecret,"/server/secret")

if __name__ == '__main__':
    app.run(debug=True)

