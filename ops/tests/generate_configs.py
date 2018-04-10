import json
from process_manager.init_configs import create_dir

PORT = 1400
LOCAL_PORT = 1122
CHAIN_HASH = 1234
CHAIN_CONFIG_DIR = "/tmp/node/m_chains"
PIPES_DIR = "/tmp/node/pipes"


def generate_conf(nodes):
    for i, n in enumerate(nodes):

        parties = []
        for j, nn in enumerate(nodes):
            if n != nn:
                parties.append(
                    {
                        "id": j,
                        "ip": nn['ip'],
                        "port": PORT
                    }
                )
        chain_conf = {
            "mChainHash": CHAIN_HASH,
            "nodes": parties
        }

        node_conf = {
            "id": i,
            "ip": n['ip'],
            "port": PORT,
            "localPort": LOCAL_PORT,
            "chainConfigDir": CHAIN_CONFIG_DIR,
            "pipesDir": PIPES_DIR,
            "redisHost": "localhost",
            "redisPort": 6378
        }

        create_dir("nodes")
        create_dir("nodes/node_{}".format(i))

        with open("nodes/node_{}/chain.json".format(i), 'w') as outfile:
            json.dump(chain_conf, outfile)

        with open("nodes/node_{}/node.json".format(i), 'w') as outfile:
            json.dump(node_conf, outfile)


if __name__ == "__main__":
    from machines import machines

    generate_conf(machines)
