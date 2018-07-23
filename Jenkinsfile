pipeline {
  agent any
  stages {
    stage('Verify Guidelines') {
      steps {
        echo 'Git URL       is ${SON_URL}'
      }
    }
  }
  environment {
    SON_URL = 'https://github.com/danghoaison91/openairinterface.git'
  }
}