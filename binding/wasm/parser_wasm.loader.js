export function loadWorker() {
  var worker = null
  if (window && window.Worker) {
    worker = new Promise((resolve, reject) => {
      var w = new Worker('parser_wasm.worker.js')
      init_worker(w).then(() => resolve(parserInterface(w)))
    })
  } else {
    throw Error('Web worker not supported')
  }
  return worker
}

export default loadWorker

function parserInterface(w) {
  return {
    parse_project: (file_ids, file_contents) =>
      parse_project(w, file_ids, file_contents),
  }
}

function init_worker(w) {
  return new Promise((resolve, reject) => {
    w.postMessage({
      action: 'init',
    })

    w.onmessage = event => {
      if (event['data']['action'] == 'loaded') resolve()
      else if (event['data']['action'] == 'parse_project_result')
        resolve_parse(event['data'])
      else throw Error('Expected loaded event !')
    }
  })
}

var _id_to_parse_promise = {}

function resolve_parse(event_data) {
  const id = event_data['id']
  if (!(id in _id_to_parse_promise)) {
    throw Error('Got an invalid parse id. No parse job queued with this id.')
  }

  const [resolve, reject] = _id_to_parse_promise[id]

  if (event_data['success']) resolve(JSON.parse(event_data['data']))
  else reject()

  delete _id_to_parse_promise[id]
}

function parse_project(w, file_ids, file_contents) {
  const id = '' + Math.floor(Math.random() * 1000000000)

  return new Promise((resolve, reject) => {
    w.postMessage({
      action: 'parse_project',
      data: {
        id,
        file_ids,
        file_contents,
      },
    })

    _id_to_parse_promise[id] = [resolve, reject]
  })
}
