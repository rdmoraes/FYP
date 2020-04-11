import React from 'react';
import Amplify from 'aws-amplify';
import awsconfig from './aws-exports';
import { withAuthenticator } from 'aws-amplify-react'
import '@aws-amplify/ui/dist/style.css';
import './index.css'
import Routes from './routes';


Amplify.configure(awsconfig);



function App() {
  return (
      <Routes />
    );
}

//export default App;
export default withAuthenticator(App, true);
