Module.print('*');
var ret;
ret = Module['ccall']('get_int', 'number'); Module.print([typeof ret, ret].join(','));
ret = ccall('get_float', 'number'); Module.print([typeof ret, ret.toFixed(2)].join(','));
ret = ccall('get_string', 'string'); Module.print([typeof ret, ret].join(','));
ret = ccall('print_int', null, ['number'], [12]); Module.print(typeof ret);
ret = ccall('print_float', null, ['number'], [14.56]); Module.print(typeof ret);
ret = ccall('print_string', null, ['string'], ["cheez"]); Module.print(typeof ret);
ret = ccall('print_string', null, ['array'], [[97, 114, 114, 45, 97, 121, 0]]); Module.print(typeof ret); // JS array
ret = ccall('print_string', null, ['array'], [new Uint8Array([97, 114, 114, 45, 97, 121, 0])]); Module.print(typeof ret); // typed array
ret = ccall('multi', 'number', ['number', 'number', 'number', 'string'], [2, 1.4, 3, 'more']); Module.print([typeof ret, ret].join(','));
var p = ccall('malloc', 'pointer', ['number'], [4]);
setValue(p, 650, 'i32');
ret = ccall('pointer', 'pointer', ['pointer'], [p]); Module.print([typeof ret, getValue(ret, 'i32')].join(','));
Module.print('*');
// part 2: cwrap
var noThirdParam = Module['cwrap']('get_int', 'number');
Module.print(noThirdParam());
var multi = Module['cwrap']('multi', 'number', ['number', 'number', 'number', 'string']);
Module.print(multi(2, 1.4, 3, 'atr'));
Module.print(multi(8, 5.4, 4, 'bret'));
Module.print('*');
// part 3: avoid stack explosion and check it's restored correctly
for (var i = 0; i < TOTAL_STACK/60; i++) {
  ccall('multi', 'number', ['number', 'number', 'number', 'string'], [0, 0, 0, '123456789012345678901234567890123456789012345678901234567890']);
}
Module.print('stack is ok.');
ccall('call_ccall_again', null);
