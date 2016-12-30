# uop-hash

Calculates the hash used by UOP files.

Since just 53-bits is the maximum supported precision by javascript with integers, this function returns an array of 32-bit unsigned integers.

# Installation

`npm install uop-hash` or `yarn add uop-hash`

# Example

```javascript
var hash = require('uop-hash');

var myHashedFilename = hash('beepboop'); // returns an uint32_t[2] (Number[2])
```

# Notes

The code listed in this was adapted from ServUO's UltimaSDK source in [FileIndex.cs:HashFileName function](https://github.com/ServUO/ServUO/blob/6754bb0bfcddaece52ef969dc39fe649a2836c6b/Ultima/FileIndex.cs#L471)
