import React, {useRef, useEffect, useState} from 'react';
import './App.css';
import {select, line, curveCardinal, axisBottom, axisRight, scaleLinear} from "d3";



function App() {
  const [data, setData] = useState([25, 30 ,45, 60, 20, 65, 75]);
  const svgRef = useRef();
  
  // will be called initially and every data change
  useEffect(()=>{
    
    //use D3 
    const svg = select(svgRef.current);
    
    const xScale = scaleLinear().domain([0,data.length-1]).range([0,300]);
    const yScale = scaleLinear().domain([0, 75]).range([150, 0]);
    
    const xAxis = axisBottom(xScale).ticks(data.length).tickFormat(index => index +1);
    const yAxis = axisRight(yScale);

    svg.select(".x-axis").call(xAxis).style("transform", "translateY(150px)");
    svg.select(".y-axis").call(yAxis).style("transform", "translatex(300px)");
    //generates the "d" attribute of a path element
    const myLine = line()
      .x((value, index)=> xScale(index))
      .y(yScale)
      .curve(curveCardinal);

    //renders path element, and attaches "d" attribute
    svg.selectAll(".line")
      .data([data])
      .join("path")
      .attr("d", myLine)
      .attr("class", "line")
      .attr("fill", "none")
      .attr("stroke", "blue")
    }, [data]);


  return (
    <React.Fragment>
      <svg ref={svgRef}>
        <g className="x-axis"/>
        <g className="y-axis"/>
      </svg>
      <br/>
      <br/>
      <br/>
      <button onClick={()=>setData(data.map(value=> value +5))}>
        Update Data
      </button>
      <button onClick={()=>setData(data.filter(value=> value < 35))}>
        Filter Data
      </button>
    </React.Fragment>
    
  );
}

export default App;
