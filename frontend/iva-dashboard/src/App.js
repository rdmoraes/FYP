import React from 'react';
import Amplify, {API} from 'aws-amplify';
import awsconfig from './aws-exports';
import { withAuthenticator } from 'aws-amplify-react'
import '@aws-amplify/ui/dist/style.css';
import './main.css'
import WelcomePage from './pages/Welcome';
import LineGraph from './pages/LineGraph'

Amplify.configure(awsconfig);



function App() {
  return (
    //<Init />
    //<LineGraph />
    <WelcomePage />
    );
}

export default App;
//export default withAuthenticator(App, true);
