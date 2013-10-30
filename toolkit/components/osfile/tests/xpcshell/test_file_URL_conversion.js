function run_test() {
  Components.utils.import("resource://gre/modules/Services.jsm");
  Components.utils.import("resource://gre/modules/osfile.jsm");
  Components.utils.import("resource://gre/modules/FileUtils.jsm");

  let isWindows = ("@mozilla.org/windows-registry-key;1" in Components.classes);

  // Test cases for filePathToURI
  let paths = isWindows ? [
    'C:\\',
    'C:\\test',
    'C:\\test\\',
    'C:\\test%2f',
    'C:\\test\\test\\test',
    'C:\\test;+%',
    'C:\\test?action=index\\',
    'C:\\test\ test',
    '\\\\C:\\a\\b\\c',
    '\\\\Server\\a\\b\\c',
    '\\\\PunctuationServer\\;,/?:@&=+$-_.!~*\'()"#'
  ] : [
    '/',
    '/test',
    '/test/',
    '/test%2f',
    '/test/test/test',
    '/test;+%',
    '/test?action=index/',
    '/test\ test',
    '/punctuation/;,/?:@&=+$-_.!~*\'()"#',
    '/CasePreserving'
  ];

  // some additional URIs to test, beyond those generated from paths
  let uris = isWindows ? [
    'file:///C:/test/',
    'file://localhost/C:/test',
    'file:///c:/test/test.txt',
    'file:///C:/foo%2f', // trailing, encoded slash
    'file:///C:/%3f%3F',
    'file:///C:/%3b%3B',
    'file:///C:/%3c%3C', // not one of the special-cased ? or ;
    'file:///C:/%78', // 'x', not usually uri encoded
    'file:///C:/test#frag', // a fragment identifier
    'file:///C:/test?action=index' // an actual query component
  ] : [
    'file:///test/',
    'file://localhost/test',
    'file:///test/test.txt',
    'file:///foo%2f', // trailing, encoded slash
    'file:///%3f%3F',
    'file:///%3b%3B',
    'file:///%3c%3C', // not one of the special-cased ? or ;
    'file:///%78', // 'x', not usually uri encoded
    'file:///test#frag', // a fragment identifier
    'file:///test?action=index' // an actual query component
  ];

  for (let path of paths) {
    // convert that to a uri using FileUtils and Services, which toFileURI is trying to model
    let file = FileUtils.File(path);
    let uri = Services.io.newFileURI(file).spec;
    do_check_eq(uri, OS.Path.toFileURI(path));

    // keep the resulting URI to try the reverse
    uris.push(uri)
  }

  for (let uri of uris) {
    // convert URIs to paths with nsIFileURI, which fromFileURI is trying to model
    let path = Services.io.newURI(uri, null, null).QueryInterface(Components.interfaces.nsIFileURL).file.path;
    do_check_eq(path, OS.Path.fromFileURI(uri));
  }
}
