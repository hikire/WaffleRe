open Jest;

open Waffle;

describe("creating streams, piping, and listning", () => {
  open Expect;
  test("stream can take and return a value", () => {
    let s = stream();
    pour(2) @@ s;
    s#peek() |> expect |> toEqual(Some(2));
  });
  test("piping a stream and receiving values", () => {
    let s = stream();
    let s2 = stream();
    pipe(s2) @@ s;
    pour(2) @@ s;
    s2#peek() |> expect |> toEqual(Some(2));
  });
  test("piped stream gets the default value", () => {
    let s = stream();
    let s2 = stream();
    pour(2) @@ s;
    pipe(s2) @@ s;
    s2#peek() |> expect |> toEqual(Some(2));
  });
  test("a listner gets notified on the default value", () => {
    let s = stream();
    let notified = ref(false);
    let listner = i => notified := i == 2;
    pour(2) @@ s;
    (listner |> on) @@ s;
    notified^ |> expect |> toBe(true);
  });
  test("a listner gets notified on value changes", () => {
    let s = stream();
    let notified = ref(false);
    let listner = i => notified := i == 3;
    pour(2) @@ s;
    (listner |> on) @@ s;
    pour(3) @@ s;
    notified^ |> expect |> toBe(true);
  });
});

describe("mapping and scanning", () => {
  open Expect;
  test("map transforms values", () => {
    let increase = (i => i + 1) |> map;
    pour(3) @@ increase;
    increase#peek() |> expect |> toEqual(Some(4));
  });
  test("mapAndPipe", () => {
    let s = stream();
    let increase = ((i => i + 1) |> mapAndPipe) @@ s;
    pour(2) @@ s;
    increase#peek() |> expect |> toEqual(Some(3));
  });
  test("scan accumlator works", () => {
    let sum = scan((acc, i) => acc + i, 1);
    pour(2) @@ sum;
    pour(3) @@ sum;
    sum#peek() |> expect |> toEqual(Some(6));
  });
  test("scanAndPipe", () => {
    let s = stream();
    pour(2) @@ s;
    let sum = scanAndPipe((acc, i) => acc + i, 0, s);
    pour(3) @@ s;
    sum#peek() |> expect |> toEqual(Some(5));
  });
});