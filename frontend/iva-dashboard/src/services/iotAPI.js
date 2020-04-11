import axios from 'axios';

const iotApi = axios.create({
    baseURL: 'https://0avipr3jd3.execute-api.us-east-1.amazonaws.com/Prod/',
    headers: {'Content-Type': 'application/json'}
});


export default iotApi;