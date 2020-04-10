import React from 'react';


import './styles.css';
import logo from '../../assets/logo.svg'


function WelcomePage(){
    return(
        <div className="main-container">
            <div className="welcome-container">
                
                    <img src={logo} alt="Engine-Analyses"/>  
                    <button className="button" type="submit">
                     Go to the dashboard!
                    </button>
                
            </div>
            
        </div>
        
    );
}

export default WelcomePage;