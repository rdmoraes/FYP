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
            data: []
        }
    }

    //Handle asynch request before render JSX
    componentDidMount(){
       let xLabel =1, yLabel=0;
       let dataPoints = [];
       apiData.get('getAnalysedData')
       .then(res => {
           return JSON.parse(res.data.body.Item.x_axis);
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
                title:{
                    text: "Frequency Domain Analyses"
                },
                axisY:{
                    includeZero: false
                },
                data : [{
                    type: "area",
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