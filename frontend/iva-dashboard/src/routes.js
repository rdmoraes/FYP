import React from 'react';
import {BrowserRouter, Route, Switch} from 'react-router-dom';

import WelcomePage from './pages/Welcome';
import DashBoard from './pages/DashBoard';

function Routes(){
    return(
    <BrowserRouter>
        <Switch>
            <Route path="/" exact component={WelcomePage} />
            <Route path="/dashboard" component={DashBoard} />
        </Switch>
    </BrowserRouter>
    );
}
export default Routes;