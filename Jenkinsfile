pipeline {

    environment {
        branch = 'udp_packeting'
        scmUrl = 'git@github.com:GalacticExchange/reliable_broadcast.git'
    }

    agent {
        dockerfile {
            filename 'Dockerfile.build'
        }
    }


    stages {

        stage('checkout git') {
            steps {
                git branch: branch, credentialsId: '6db36386-1c0a-4d7e-b10a-88b0a7a6826d', url: scmUrl
            }
        }

        stage('Build') {
            steps {
                sh 'cd ops/ansible && ./build.sh'
            }
        }
 /*
        stage('Deploy') {
            steps {
                sh 'cd ops/ansible/inventories && gpg --passphrase ${INVENTORY_SECRET} inventory.gpg'
                sh 'cd ops/ansible && ./redeploy.sh'
            }
        }
 */
    }

}