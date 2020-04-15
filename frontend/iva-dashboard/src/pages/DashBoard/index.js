import React, {useState} from 'react'
import FFTChart from '../../components/FFTChart'
import RawDataChart from '../../components/RawDataChart'
import {FiRefreshCcw} from 'react-icons/fi'

import logoText from '../../assets/logoText.svg'
import iotApi from '../../services/iotAPI'

import './styles.css';

function DashBoard(){

    const[isRefresh, setRefresh] =useState(false);

    async function handleTakeSampleButton(){
        
        try{
            const response = await iotApi.post('publish',{"command": "takeSample"});   
            const date = new Date(parseInt(response.data.body.timestamp));
            
            console.log("Request status",response.data.body.status);
            alert('Request performed ID: '+ response.data.body.timestamp + " " + date.toString());
            
        }catch(err){
            alert('IoT device connection fail!');
        }

    }

    function refreshPage() {
        window.location.reload(false);
        setRefresh(true);
        console.log(isRefresh);
    }

    return(
        <div className="dashboard-container">
            <header>
                <img src={logoText} alt="iVA-Logo"/>
                <span> Data Analyses</span>
                <button 
                    className="button" 
                    onClick={handleTakeSampleButton}>
                        New data sample
                </button>
                <button 
                    className="button-refresh" 
                    type="button"
                    onClick={refreshPage}>
                    <FiRefreshCcw size={18} color="#2090db75" />
                </button>                
            </header>
            <ul className="graph-container">
                <li>
                    <strong>Time Domain (X - axis) </strong>
                    <RawDataChart 
                    axisDirection="x-axis-raw" 
                    lineColor="#ffb677" 
                    displayRefresh={isRefresh}
                    labelAxisY="Acceleration (g)"
                    labelAxisX="Time (ms)"/> 
                </li>
                <li>
                    <strong>Frequency Domain (X - axis) </strong>
                    <FFTChart 
                    axisDirection="x-axis" 
                    lineColor="#ffb677" 
                    displayRefresh={isRefresh}
                    labelAxisY="Amplitude (dB)"
                    labelAxisX="Frequency (Hz)"/> 
                </li>
                <li>
                    <strong>Time Domain (Y - axis) </strong>
                    <RawDataChart 
                    axisDirection="y-axis-raw" 
                    lineColor="#3b6978" 
                    displayRefresh={isRefresh}
                    labelAxisY="Acceleration (g)"
                    labelAxisX="Time (ms)"/> 
                </li>
                <li>
                    <strong>Frequency Domain (Y - axis) </strong>
                    <FFTChart 
                    axisDirection="y-axis" 
                    lineColor="#3b6978" 
                    displayRefresh={isRefresh}
                    labelAxisY="Amplitude (dB)"
                    labelAxisX="Frequency (Hz)"/> 
                </li>
                <li>
                    <strong>Time Domain (Z - axis) </strong>
                    <RawDataChart 
                    axisDirection="z-axis-raw" 
                    lineColor="#8566aa" 
                    displayRefresh={isRefresh}
                    labelAxisY="Acceleration (g)"
                    labelAxisX="Time (ms)"/> 
                </li>
                <li>
                    <strong>Frequency Domain (Z - axis) </strong>
                    <FFTChart 
                    axisDirection="z-axis" 
                    lineColor="#8566aa" 
                    displayRefresh={isRefresh}
                    labelAxisY="Amplitude (dB)"
                    labelAxisX="Frequency (Hz)"/> 
                </li>
                                
            </ul>
        </div>
    );
}

export default DashBoard;