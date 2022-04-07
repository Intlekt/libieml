# Parser Wasm

Wasm Parser for [IEML](https://intlekt.io/ieml/).

This version is not up to date. [Contact us](https://intlekt.io/contact/) if you want a version up to date.

## Installation

```bash
npm install @intlekt/parser-wasm
yarn add @intlekt/parser-wasm
```

## Usage

```html
<html>
  <head></head>
  <body style="display: grid; grid-template-columns: 50% auto">
    <textarea id="area"></textarea>
    <div id="result"></div>
    <script type="module">
      import { loadWorker } from '@intlekt/parser-wasm'
      loadWorker().then(worker => {
        document.getElementById('area').addEventListener('change', e => {
          worker.parse_project([''], [e.target.value]).then(res => {
            document.getElementById('result').textContent = JSON.stringify(res)
          })
        })
      })
    </script>
  </body>
</html>
```
