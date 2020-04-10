import React from 'react';
import {BrowserRouter, Route, Switch} from 'react-router-dom';

import WelcomePage from './pages/Welcome';

function Routes(){
    return(
    <BrowserRouter>
        <Switch>
            <Route path="/" component={WelcomePage} />
        </Switch>
    </BrowserRouter>
    );
}
export default Routes;