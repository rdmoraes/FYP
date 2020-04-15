import React from 'react';
import CanvasJsReact from '../../assets/canvasjs.react';
import apiData from '../../services/dataApi'

import './styles.css';


var CanvasJsChart = CanvasJsReact.CanvasJSChart;
var Component = React.Component;


class FFTChart extends Component{
    
    constructor(props){
        super(props);

        this.state = {
            error: null,
            isLoaded: false,
            data: [],
            axisDirection: props.axisDirection,
            lineColor: props.lineColor,
            labelAxisY: props.labelAxisY,
            labelAxisX: props.labelAxisX
            
        }
    }

    //Handle asynch request before render JSX
    componentDidMount(){
             
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
      
        })
        .then(data =>{
            const freqSample = 2000;
            const numSamples = data.length;
            const freqRatio = freqSample/(numSamples)
            let yLabel=0;
            let xLabel = 0;
            
            for(var i=0; i <data.length/2; i++){
                yLabel = data[i];
                dataPoints.push({x:xLabel, y: yLabel});
                xLabel = xLabel+ freqRatio;
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
                   title: this.state.labelAxisY,
                },
                axisX:{
                    title: this.state.labelAxisX,
                    includezero: true,
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

export default FFTChart;
