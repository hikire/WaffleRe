# Waffle.re

Waffle is simple streams library for reasonML, inspired by [flyd](https://github.com/paldepind/flyd), and was originally created for implementing [Meiosis](https://meiosis.js.org/) for state management - still haven't used it for that yet :innocent: .

Since Waffle is still under development, it lacks many of the features in flyd(e.g. combining streams, atomic updates, ...), so you're welcome to help :smile:

## Example:

```reason
let input = Waffle.stream();

let inc = Waffle.map(i => i + 1);

(inc |> Waffle.pipe) @@ input;

((i => Js.log(i)) |> Waffle.on) @@ inc; /* adding a listner */

input#pour(1); /* inc logs 2 */
Js.log(input#peek() == Some(1)); /* logs true */

(5 |> Waffle.pour) @@ inc; /* inc logs 6 */

let sum = Waffle.scanAndPipe((acc, v) => acc + v, 0, inc);

((i => Js.log(i)) |> Waffle.on) @@ sum; /* sum logs 6 (from inc + 0) */

Js.log("acc sum:");

(2 |> Waffle.pour) @@ sum; /* sum logs 8 */
(5 |> Waffle.pour) @@ sum; /* sum logs 13 */
```

## Installation

```sh
npm install --save reason-waffle
```

Then add `reason-waffle` to `bs-dependencies` in your `bsconfig.json`:

```js
{
  ...
  "bs-dependencies": ["reason-waffle"]
}
```

### More... well, you know :sweat_smile:
