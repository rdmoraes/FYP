/*
  This funciton is triggered by AWS API Gateway and returns fft data from DynamoDB
  
  Renan Moraes
*/

const AWS = require('aws-sdk');
const dynamo = new AWS.DynamoDB.DocumentClient();


exports.handler = async (event, context) => {
    //console.log('Received event:', JSON.stringify(event, null, 2));

    let body;
    let statusCode = '200';
    const headers = {
        'Content-Type': 'application/json',
        'Access-Control-Allow-Headers' : 'Origin, X-Requested-With, Content-Type, Accept',
        'Access-Control-Allow-Origin' : '*'
    };

    try {
        const params = { 
            Key: {
            "id": "lastResult",
            },
            TableName: "analysedData"
        };
             
        body = await dynamo.get(params).promise();
          
        
    } catch (err) {
        statusCode = '400';
        body = err.message;
    } 
    
    return {
        statusCode,
        body,
        headers,
    };
};