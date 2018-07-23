pipeline {
  agent any
  stages {
    stage('Verify Guidelines') {
      steps {
        echo 'Git URL       is ${SON_URL}'
      }
    }
    stage('Edit JenkinFile') {
      steps {
        echo 'Edit JenkinFile'
      }
    }
  }
  environment {
    SON_URL = 'https://github.com/danghoaison91/openairinterface.git'
    GIT_BRANCH = 'develop-nr'
  }
}