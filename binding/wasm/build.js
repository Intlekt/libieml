import { copyFileSync, constants } from 'fs'
import path from 'path'
import { fileURLToPath } from 'url'

const __dirname = path.dirname(fileURLToPath(import.meta.url))

const buildPath = path.resolve(__dirname, '../../build/binding/wasm')

const files = ['parser_wasm.wasm', 'parser_wasm.js']

files.forEach(file => {
  copyFileSync(path.join(buildPath, file), path.join(__dirname, file))
})
