const express = require('express');
const path = require('path');
const app = express();
 
// Server port
const PORT = 3000;

app.get('/', (request, response) => {
	response.send('Hello from RPiZ2W!');
	console.log('root');
});


app.get('/firmware/:filename', (request, response) => {
	const filePath = __dirname + "/firmware/" + request.params.filename;
	console.log('Request ' + filePath + '\n');
    response.download(
		filePath,
		request.params.filename,
		(err)=>{
            if (err) {
				console.error("Problem on download firmware: ", err);
                response.send({
                    error : err,
                    msg   : "Problem downloading the file"
                })
			}
    });
	console.log('downloaded\n');
});

app.listen(PORT, () => console.log('Listening on port '+PORT+'\n'));