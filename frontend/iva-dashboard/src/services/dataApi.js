import axios from 'axios';

const dataApi = axios.create({
    baseURL: 'https://sqji67vp61.execute-api.us-east-1.amazonaws.com/dev/'
});




export default dataApi;
