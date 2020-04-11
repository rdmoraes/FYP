import React from 'react';
import CanvasJsReact from '../../assets/canvasjs.react';
import apiData from '../../services/dataApi'

import './styles.css';


var CanvasJsChart = CanvasJsReact.CanvasJSChart;
var Component = React.Component;


class LineGraph extends Component{
    
    constructor(props){
        super(props);

        this.state = {
            error: null,
            isLoaded: false,
            data: [],
            axisDirection: props.axisDirection,
            lineColor: props.lineColor,
            
        }
    }

    //Handle asynch request before render JSX
    componentDidMount(){
       let xLabel =1, yLabel=0;
       let dataPoints = [];
       let data;
              
        apiData.get('getAnalysedData')      
       .then(res => {
        switch (this.state.axisDirection){
            case 'x-axis':
                data = JSON.parse(res.data.body.Item.x_axis);
                break;
            case 'y-axis':
                data = JSON.parse(res.data.body.Item.y_axis);
                break;
            case 'z-axis':
                data = JSON.parse(res.data.body.Item.z_axis);
                break;
            case 'x-axis-raw':
                data = JSON.parse(res.data.body.Item.x_axis_raw);
                break;
            case 'y-axis-raw':
                data = JSON.parse(res.data.body.Item.y_axis_raw);
                break;
            case 'z-axis-raw':
                data = JSON.parse(res.data.body.Item.z_axis_raw);
                break;
            default:
                throw new Error(`Unsupported tag name "${this.state.axisDirection}"`);
        }
        return data;
        // if(this.state.axisDirection === "x-axis"){
        //     return JSON.parse(res.data.body.Item.x_axis);
        // }
        // else if(this.state.axisDirection ==="y-axis"){
        //     return JSON.parse(res.data.body.Item.y_axis);
        // }
            
        // return JSON.parse(res.data.body.Item.z_axis);

        })
        .then(data =>{
            for(var i=0; i <data.length; i++){
                yLabel = data[i];
                dataPoints.push({x:xLabel, y: yLabel});
                xLabel++;
            }
            this.setState({
                isLoaded: true,
                data : dataPoints})
            
       },
       (error) =>{
           this.setState({
               isLoaded: true,
               error
           });
       });
     
   }

    render(){
        if(this.state.error){
            return(
                <div>Failed while retriving data. Error: {this.state.error.message}</div>
            )
        }else if(!this.state.isLoaded){
            return(
                <div>Plotting data ... </div>
            )
        }else{
            const options = {
                theme: "light2", 
                animationEnabled: true,
                zoomEnabled: true,
                axisY:{
                    includeZero: false,
                    title: "Magnitude"
                },
                axisX:{
                    title: "Frequency"
                },
                data : [{
                    type: "column",
                    color: this.state.lineColor,
                    dataPoints: this.state.data
                }]
            }        
            return(
                <div>
                    <CanvasJsChart options = {options} />
                </div>
            )
        }
    }
}

export default LineGraph;
//module.exports = Dash;