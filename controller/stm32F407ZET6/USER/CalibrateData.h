#ifndef _CalibrateData_H
#define _CalibrateData_H
//0->��Ĵָ 1->ʳָ 2->��ָ 3->����ָ 4->СĴָ 


int mesDIPAngle[5][3] = {
	 //DIP-min  DIP-max  DIP-size  
 	{3820,5580,2760},
	{4760,5900,1140},
	{4865,6465,1600},
	{4950,5500,550},
	{4400,5450,1050}
};
int mesPIPAngle[5][3] = {
	  //PIP-min  PIP-max  PIP-size  
 	{4900,5680,780},
	{4500,6000,1500},
	{4240,5700,1460},
	{4100,5780,1680},
	{4800,5700,900}
};
int mesMCPAngle[5][3] = {
	 //MCP-min  MCP-max  MCP-size  
 	{0,0,0},
	{4880,5800,920},
	{4900,5650,750},
	{4780,5160,380},
	{4800,5150,350},
};



float DIPSesAngle[5][300] = {
	{78.16,76.95,75.82,74.75,73.76,72.82,71.95,71.14,70.38,69.67,69,68.39,67.82,67.28,66.79,66.33,65.9,65.51,65.14,64.8,64.48,64.18,63.9,63.64,63.4,63.17,62.95,62.74,62.54,62.35,62.17,61.98,61.8,61.63,61.45,61.27,61.08,60.9,60.71,60.51,60.31,60.1,59.88,59.65,59.42,59.17,58.91,58.64,58.36,58.06,57.75,57.42,57.09,56.73,56.37,55.98,55.59,55.17,54.74,54.3,53.84,53.36,52.87,52.36,51.84,51.3,50.74,50.18,49.59,48.99,48.38,47.75,47.11,46.46,45.79,45.11,44.42,43.72,43,42.28,41.54,40.8,40.05,39.29,38.52,37.74,36.96,36.17,35.37,34.58,33.77,32.97,32.16,31.35,30.54,29.73,28.92,28.11,27.3,26.49,25.69,24.89,24.1,23.31,22.53,21.75,20.98,20.22,19.47,18.73,18,17.28,16.57,15.87,15.18,14.51,13.85,13.21,12.58,11.97,11.37,10.79,10.22,9.67,9.14,8.63,8.14,7.66,7.2,6.76,6.34,5.94,5.56,5.2,4.85,4.53,4.22,3.94,3.67,3.42,3.19,2.98,2.79,2.61,2.45,2.31,2.18,2.07,1.97,1.88,1.81,1.75,1.7,1.66,1.63,1.61,1.59,1.58,1.58,1.57,1.57,1.57,1.56,1.55,1.53,1.51,1.47,1.42,1.36,1.29,1.19,1.07,0.93,0.76,0.56,0.33,0.07
},
	{52.56,51.2,49.87,48.56,47.28,46.03,44.81,43.61,42.44,41.3,40.18,39.09,38.03,36.99,35.97,34.98,34.02,33.08,32.16,31.27,30.4,29.55,28.72,27.92,27.14,26.38,25.64,24.92,24.23,23.55,22.89,22.25,21.64,21.04,20.46,19.89,19.35,18.82,18.31,17.82,17.35,16.89,16.44,16.02,15.61,15.21,14.83,14.46,14.1,13.76,13.44,13.13,12.82,12.54,12.26,12,11.75,11.51,11.28,11.06,10.85,10.65,10.46,10.28,10.11,9.95,9.8,9.65,9.52,9.39,9.27,9.15,9.05,8.95,8.85,8.77,8.68,8.61,8.54,8.47,8.41,8.36,8.3,8.26,8.21,8.17,8.13,8.1,8.07,8.04,8.01,7.99,7.97,7.95,7.93,7.91,7.9,7.88,7.87,7.86,7.84,7.83,7.82,7.81,7.79,7.78,7.77,7.75,7.74,7.72,7.7,7.68,7.66,7.64,7.62
},
	{39.28,37.95,36.68,35.47,34.32,33.22,32.17,31.17,30.22,29.31,28.45,27.62,26.84,26.09,25.38,24.7,24.06,23.44,22.85,22.3,21.76,21.25,20.77,20.31,19.87,19.44,19.04,18.66,18.29,17.93,17.59,17.27,16.95,16.65,16.36,16.08,15.81,15.55,15.3,15.05,14.81,14.58,14.35,14.13,13.92,13.71,13.5,13.3,13.1,12.9,12.71,12.52,12.33,12.14,11.96,11.78,11.59,11.41,11.24,11.06,10.88,10.71,10.53,10.36,10.19,10.01,9.84,9.67,9.5,9.33,9.16,8.99,8.83,8.66,8.5,8.33,8.17,8.01,7.85,7.69,7.53,7.37,7.22,7.06,6.91,6.76,6.61,6.47,6.32,6.18,6.04,5.91,5.77,5.64,5.52,5.39,5.27,5.15,5.03,4.92,4.81,4.71,4.6,4.5,4.41,4.32,4.23,4.14,4.06,3.99,3.92,3.85,3.78,3.72,3.66,3.61,3.56,3.52,3.48,3.44,3.4,3.37,3.35,3.32,3.3,3.29,3.28,3.27,3.26,3.26,3.26,3.26,3.26,3.27,3.28,3.29,3.31,3.32,3.34,3.36,3.38,3.4,3.42,3.44,3.47,3.49,3.51,3.54,3.56,3.58,3.6,3.62,3.63,3.65,3.66,3.67,3.68,3.69,3.69,3.69,3.68
},
	{39.39,38.35,37.22,36.01,34.75,33.44,32.12,30.79,29.46,28.14,26.84,25.58,24.34,23.14,21.99,20.87,19.8,18.77,17.79,16.86,15.96,15.11,14.3,13.52,12.78,12.08,11.41,10.77,10.16,9.57,9.01,8.47,7.96,7.47,6.99,6.54,6.11,5.7,5.3,4.93,4.58,4.26,3.95,3.68,3.42,3.2,3,2.84,2.7,2.6,2.53,2.49,2.48,2.51,2.57,2.65
},
	{50.65,49.95,49.24,48.51,47.78,47.04,46.28,45.53,44.76,44,43.23,42.45,41.68,40.91,40.13,39.36,38.59,37.83,37.06,36.3,35.55,34.8,34.06,33.32,32.6,31.88,31.16,30.46,29.76,29.07,28.4,27.73,27.07,26.42,25.79,25.16,24.54,23.94,23.34,22.76,22.19,21.63,21.08,20.54,20.01,19.49,18.99,18.49,18.01,17.54,17.08,16.63,16.18,15.75,15.33,14.92,14.52,14.13,13.75,13.38,13.02,12.67,12.32,11.99,11.66,11.34,11.03,10.73,10.43,10.15,9.87,9.59,9.33,9.07,8.82,8.57,8.33,8.1,7.87,7.65,7.43,7.22,7.02,6.82,6.63,6.44,6.25,6.08,5.91,5.74,5.58,5.42,5.27,5.13,4.99,4.86,4.73,4.61,4.5,4.39,4.29,4.2,4.12,4.04,3.97,3.91
}
};

float PIPSesAngle[5][220] = {
	{33.21,32.27,31.35,30.46,29.6,28.76,27.96,27.17,26.42,25.69,24.98,24.3,23.64,23.01,22.39,21.8,21.23,20.68,20.15,19.63,19.14,18.66,18.19,17.75,17.31,16.89,16.48,16.08,15.69,15.31,14.94,14.58,14.22,13.87,13.53,13.19,12.85,12.52,12.19,11.86,11.53,11.2,10.87,10.55,10.22,9.88,9.55,9.22,8.88,8.54,8.2,7.85,7.51,7.16,6.81,6.46,6.11,5.75,5.4,5.05,4.7,4.35,4,3.66,3.32,3,2.68,2.37,2.07,1.78,1.51,1.26,1.03,0.82,0.63,0.47,0.34,0.24,0.18
},
	{78.95,78.69,78.44,78.2,77.97,77.72,77.48,77.22,76.95,76.66,76.35,76.02,75.67,75.29,74.89,74.45,73.99,73.5,72.97,72.42,71.83,71.21,70.56,69.88,69.16,68.42,67.65,66.84,66.01,65.16,64.27,63.36,62.43,61.48,60.5,59.5,58.49,57.46,56.41,55.35,54.27,53.19,52.1,50.99,49.89,48.77,47.66,46.54,45.42,44.3,43.19,42.07,40.97,39.87,38.78,37.69,36.62,35.56,34.51,33.48,32.46,31.45,30.47,29.49,28.54,27.61,26.69,25.8,24.93,24.08,23.24,22.44,21.65,20.89,20.15,19.43,18.74,18.07,17.42,16.8,16.2,15.63,15.07,14.54,14.03,13.55,13.09,12.64,12.22,11.82,11.44,11.08,10.74,10.42,10.12,9.83,9.56,9.31,9.07,8.85,8.64,8.44,8.26,8.09,7.93,7.78,7.64,7.51,7.39,7.27,7.16,7.06,6.97,6.87,6.79,6.7,6.62,6.54,6.47,6.39,6.32,6.24,6.17,6.09,6.01,5.94,5.86,5.77,5.69,5.6,5.51,5.42,5.33,5.23,5.13,5.02,4.92,4.81,4.69,4.58,4.46,4.34,4.22,4.09,3.97,3.84,3.71,3.59,3.46,3.33,3.21
},
	{88.31,86.77,85.34,84.02,82.79,81.65,80.6,79.62,78.72,77.89,77.13,76.42,75.77,75.17,74.62,74.11,73.64,73.2,72.8,72.42,72.07,71.74,71.43,71.14,70.86,70.59,70.33,70.08,69.83,69.58,69.34,69.09,68.84,68.58,68.32,68.05,67.78,67.49,67.19,66.88,66.56,66.22,65.87,65.5,65.12,64.72,64.31,63.88,63.43,62.96,62.48,61.98,61.46,60.92,60.37,59.8,59.21,58.6,57.98,57.34,56.69,56.01,55.33,54.63,53.91,53.18,52.43,51.68,50.91,50.12,49.33,48.53,47.72,46.89,46.06,45.22,44.38,43.53,42.67,41.81,40.94,40.07,39.2,38.32,37.45,36.57,35.69,34.82,33.94,33.07,32.21,31.34,30.48,29.63,28.78,27.94,27.11,26.28,25.46,24.65,23.85,23.07,22.29,21.52,20.77,20.02,19.29,18.58,17.87,17.18,16.51,15.85,15.2,14.57,13.96,13.36,12.77,12.21,11.65,11.12,10.6,10.1,9.62,9.15,8.7,8.26,7.84,7.44,7.06,6.69,6.33,6,5.67,5.37,5.08,4.8,4.54,4.29,4.06,3.84,3.63,3.43,3.25,3.08,2.92,2.77,2.63
},
	{89.97,88.44,87.01,85.68,84.44,83.28,82.2,81.2,80.26,79.39,78.58,77.82,77.12,76.46,75.85,75.28,74.74,74.24,73.76,73.31,72.89,72.48,72.1,71.73,71.37,71.02,70.68,70.35,70.02,69.69,69.36,69.04,68.71,68.37,68.03,67.69,67.33,66.97,66.6,66.22,65.82,65.42,65,64.56,64.12,63.66,63.18,62.69,62.19,61.66,61.13,60.58,60.01,59.42,58.83,58.21,57.58,56.94,56.28,55.6,54.92,54.22,53.5,52.77,52.04,51.28,50.52,49.75,48.96,48.17,47.37,46.56,45.74,44.91,44.08,43.24,42.4,41.55,40.7,39.85,38.99,38.13,37.27,36.41,35.56,34.7,33.84,32.99,32.14,31.29,30.45,29.61,28.78,27.96,27.14,26.33,25.53,24.74,23.95,23.18,22.42,21.67,20.93,20.2,19.48,18.78,18.09,17.42,16.75,16.11,15.47,14.86,14.25,13.66,13.09,12.54,12,11.47,10.97,10.48,10,9.54,9.1,8.68,8.27,7.88,7.5,7.14,6.8,6.47,6.16,5.86,5.58,5.31,5.06,4.82,4.6,4.39,4.19,4.01,3.83,3.67,3.53,3.39,3.26,3.15,3.04,2.94,2.85,2.77,2.69,2.62,2.56,2.5,2.45,2.4,2.35,2.31,2.27,2.22,2.18,2.14,2.1,2.05,2.01,1.96,1.9,1.84,1.78
},
	{95.58,93.91,92.24,90.57,88.91,87.26,85.62,83.98,82.35,80.73,79.12,77.51,75.92,74.33,72.76,71.2,69.65,68.11,66.58,65.07,63.57,62.08,60.61,59.15,57.71,56.28,54.86,53.46,52.08,50.71,49.36,48.03,46.71,45.41,44.13,42.86,41.62,40.39,39.18,37.98,36.81,35.65,34.51,33.39,32.29,31.21,30.15,29.11,28.09,27.08,26.1,25.13,24.19,23.26,22.36,21.47,20.6,19.75,18.93,18.12,17.33,16.56,15.81,15.07,14.36,13.67,12.99,12.33,11.7,11.08,10.47,9.89,9.33,8.78,8.25,7.73,7.24,6.76,6.29,5.84,5.41,5,4.59,4.21,3.84,3.48,3.14,2.81,2.49,2.19,1.9
}
};

float MCPSesAngle[5][140] = {
	{0},
	{65.06,64.35,63.53,62.61,61.6,60.51,59.37,58.17,56.93,55.66,54.37,53.07,51.76,50.44,49.14,47.84,46.56,45.3,44.06,42.85,41.68,40.53,39.42,38.34,37.3,36.3,35.33,34.41,33.52,32.68,31.87,31.09,30.36,29.66,28.99,28.36,27.76,27.19,26.65,26.14,25.65,25.19,24.75,24.33,23.93,23.54,23.18,22.82,22.48,22.15,21.83,21.51,21.21,20.91,20.61,20.32,20.03,19.74,19.46,19.17,18.88,18.6,18.31,18.02,17.73,17.44,17.15,16.85,16.56,16.27,15.98,15.69,15.4,15.11,14.83,14.55,14.28,14.01,13.75,13.49,13.25,13.01,12.79,12.57,12.37,12.18,12.01,11.85,11.7,11.57,11.45,11.34,11.26
},
	{73.16,71.24,69.3,67.37,65.45,63.56,61.7,59.89,58.13,56.43,54.79,53.22,51.71,50.27,48.9,47.6,46.37,45.2,44.1,43.07,42.09,41.17,40.31,39.49,38.73,38,37.32,36.67,36.05,35.46,34.89,34.34,33.81,33.29,32.78,32.28,31.79,31.3,30.8,30.31,29.81,29.31,28.8,28.29,27.77,27.25,26.72,26.18,25.65,25.1,24.56,24.02,23.47,22.93,22.4,21.87,21.36,20.85,20.36,19.88,19.43,18.99,18.57,18.17,17.8,17.45,17.12,16.82,16.54,16.28,16.04,15.81,15.59,15.38,15.16,14.94
},
	{64.09,63.61,63.15,62.71,62.27,61.83,61.38,60.89,60.38,59.82,59.21,58.55,57.83,57.04,56.18,55.25,54.23,53.13,51.95,50.68,49.32,47.87,46.33,44.71,42.99,41.19,39.31,37.35,35.31,33.2,31.02,28.78,26.49,24.14,21.75,19.33,16.88,14.41,11.93
},
	{49.2,48.48,47.69,46.84,45.93,44.96,43.95,42.9,41.8,40.68,39.52,38.34,37.14,35.92,34.69,33.46,32.22,30.99,29.77,28.56,27.36,26.19,25.04,23.93,22.85,21.81,20.82,19.88,18.99,18.16,17.4,16.7,16.08,15.53,15.07,14.69
}
};

#endif