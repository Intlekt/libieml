export function loadWorker() {
    var worker = null;
    if (window && window.Worker) {
        worker = new Promise((resolve, reject) => {
            var w = new Worker('parser_wasm.worker.js');
            init_worker(w).then(() => resolve(parserInterface(w)))
        });
    } else {
        throw Error("Web worker not supported");
    }
    return worker;
}

function parserInterface(w) {
    return {
        'parse_project': (file_ids, file_contents) => parse_project(w, file_ids, file_contents)
    }
}


function init_worker(w) {
    return new Promise((resolve, reject) => {
        w.postMessage({
            action: "init"
        })

        w.onmessage = (event) => {
            if (event['data']['action'] == 'loaded') 
                resolve();
            else 
                throw Error("Expected loaded event !");
        }
    })
}

function parse_project(w, file_ids, file_contents) {    
    return new Promise((resolve, reject) => {
        w.postMessage({
            action: "parse_project",
            data: {
                file_ids, 
                file_contents
            }
        });
    
        w.onmessage = (event) => {
            if (event['data']['action'] == 'parse_project_result') 
                resolve(JSON.parse(event['data']['data']));
            else (event['data']['action'] == 'parse_project_error')
                reject();
        };
    })
}