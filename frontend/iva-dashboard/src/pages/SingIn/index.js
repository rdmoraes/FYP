import React from 'react';
import{FiChevronRight} from 'react-icons/fi'

import './styles.css';
import logo from '../../assets/logo.svg'
import logoText from '../../assets/logoText.svg'

function SingIn(){
    return(
        <div className="welcome-container">
            <section className="form">
                <h1>iVA</h1> 
                <img src={logo} alt="Engine-Analyses"/>  
                <a href="/register">
                <FiChevronRight size={16} color="#004aad"/>
                Go to dashboard!
                </a>
            </section>
        </div>
        
    );
}

export default SingIn;