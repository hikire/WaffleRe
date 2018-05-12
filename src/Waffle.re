type t('a, 'b) = {
  .
  on: ('b => unit) => unit,
  pour: 'a => unit,
  peek: unit => option('b),
};

let streamBase =
    (~transformer as transform: (option('b), option('a)) => option('b), ())
    : t('a, 'b) => {
  val value = ref(transform(None, None));
  val listeners = ref([]);
  pub on = listner => {
    listeners := [listner, ...listeners^];
    switch (value^) {
    | Some(output) => listner(output)
    | None => ()
    };
  };
  pub pour = chunk => {
    value := transform(value^, Some(chunk));
    switch (value^) {
    | Some(transformedChunk) =>
      Belt.List.forEach(listeners^, map => map(transformedChunk))
    | None => ()
    };
  };
  pub peek = () => value^
};

let wrapScanAsTransformer =
    (
      fn: ('b, 'a) => 'b,
      defautAccValue: 'b,
      acc: option('b),
      chunk: option('a),
    )
    : option('b) =>
  switch (chunk) {
  | Some(c) =>
    switch (acc) {
    | Some(acc) => Some(fn(acc, c))
    | None => Some(fn(defautAccValue, c))
    }
  | None => Some(defautAccValue)
  };

let createTransfomer =
    (fn: 'a => 'b, _acc: option('b), chunk: option('a))
    : option('b) =>
  switch (chunk) {
  | Some(c) => Some(fn(c))
  | None => None
  };

let stream = () => streamBase(~transformer=createTransfomer(i => i), ());

let on = (fn: 'b => 'c, stream: t('a, 'b)) => stream#on(fn);

let pipe = (pipedStream, inputStream) => inputStream#on(pipedStream#pour);

let pour = (chunk: 'a, stream: t('a, 'b)) => stream#pour(chunk);

let map = (cb: 'a => 'b) : t('a, 'b) =>
  streamBase(~transformer=createTransfomer(cb), ());

let mapAndPipe = (cb: 'b => 'c, inputStream: t('a, 'b)) : t('b, 'c) => {
  let mapperStream = map(cb);
  (mapperStream |> pipe) @@ inputStream;
  mapperStream;
};

let scan = (cb: ('b, 'a) => 'b, default: 'b) : t('a, 'b) =>
  streamBase(~transformer=wrapScanAsTransformer(cb, default), ());

let scanAndPipe =
    (cb: ('b, 'a) => 'b, default: 'b, inputStream: t('c, 'a))
    : t('a, 'b) => {
  let scannerStream = scan(cb, default);
  (scannerStream |> pipe) @@ inputStream;
  scannerStream;
};