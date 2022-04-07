self.addEventListener('message', event => {
  const action = event['data']['action']

  if (action == 'init') {
    importScripts('parser_wasm.js')
    Module.onRuntimeInitialized = send_wasm_loaded
  } else if (action == 'parse_project') {
    const file_ids = new Module.StringList()
    for (const v of event['data']['data']['file_ids']) file_ids.push_back('')

    const file_contents = new Module.StringList()
    for (const c of event['data']['data']['file_contents'])
      file_contents.push_back(c)

    var res
    try {
      res = Module.parse_project(file_ids, file_contents)
    } catch (e) {
      send_parse_project_error(event['data']['data']['id'])
      return
    }
    send_parse_project_result(res, event['data']['data']['id'])
  } else {
    throw Error(`Worker received invalid command "${event['data']}"`)
  }
})

function send_wasm_loaded() {
  self.postMessage({
    action: 'loaded',
  })
}

function send_parse_project_error(parse_id) {
  self.postMessage({
    action: 'parse_project_result',
    success: false,
    id: parse_id,
  })
}

function send_parse_project_result(res, parse_id) {
  self.postMessage({
    action: 'parse_project_result',
    success: true,
    data: res,
    id: parse_id,
  })
}
