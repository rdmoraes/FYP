import React from 'react';
import {Link} from 'react-router-dom'


import './styles.css';
import logo from '../../assets/logo.svg'


function WelcomePage(){
    return(
        <div className="main-container">
            <div className="welcome-container">
                
                    <img src={logo} alt="Engine-Analyses"/>
                    <Link to="/dashboard" className="button">Go to the dashboard!</Link>  
                                   
            </div>
            
        </div>
        
    );
}

export default WelcomePage;