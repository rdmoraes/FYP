import React from 'react'
import LineGraph from '../../components/LineGraph'
import {FiRefreshCcw} from 'react-icons/fi'

import logoText from '../../assets/logoText.svg'

import './styles.css';

function DashBoard(){
    return(
        <div className="dashboard-container">
            <header>
                <img src={logoText} alt="iVA-Logo"/>
                <span> Data Analyses</span>
                <button className="button">Take Sample</button>
                <button className="button-refresh" type="button">
                    <FiRefreshCcw size={18} color="#2090db75" />
                </button>
            </header>
            <div className="graph-container">
                <LineGraph/>
                <LineGraph/>
            </div>
        </div>
    );
}

export default DashBoard;