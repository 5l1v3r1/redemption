#!/usr/bin/python -O

import sys
import ctypes
import ctypes.util

from ctypes import CFUNCTYPE, c_ulong, c_ulonglong, c_int, c_char_p, c_uint16, py_object, c_uint, c_uint32, c_uint64, c_float, c_void_p, POINTER, Structure

#pathlib = '/usr/local/lib/libredcryptofile.so'
pathlib = './bin/clang-linux-3.8.1/debug/libredcryptofile.so'

hmac_key_str = '\xe3\x8d\xa1\x5e\x50\x1e\x4f\x6a\x01\xef\xde\x6c\xd9\xb3\x3a\x3f\x2b\x41\x72\x13\x1e\x97\x5b\x4c\x39\x54\x23\x14\x43\xae\x22\xae'

key_str = '\x56\x3e\xb6\xe8\x15\x8f\x0e\xed\x2e\x5f\xb6\xbc\x28\x93\xbc\x15\x27\x0d\x7e\x78\x15\xfa\x80\x4a\x72\x3e\xf4\xfb\x31\x5f\xf4\xb2'

GETHMACKEY = CFUNCTYPE(c_int, c_void_p)
GETTRACEKEY = CFUNCTYPE(c_int, c_char_p, c_int, c_void_p)

def get_hmac_key(resbuf):
    ctypes.memmove(resbuf, hmac_key_str, 32)
    return 0

def get_trace_key(base, lg, resbuf, flag):
    name = base[:lg]
    ctypes.memmove(resbuf, key_str, 32)
    return 0

get_hmac_key_func = GETHMACKEY(get_hmac_key)
get_trace_key_func = GETTRACEKEY(get_trace_key)

try:
    lib = ctypes.CDLL(pathlib)
    print("load {path} OK".format(path=pathlib))

# RedCryptoWriterHandle * redcryptofile_new(
#    int with_encryption, int with_checksum, get_hmac_key_prototype * hmac_fn, get_trace_key_prototype * trace_fn)
    lib.redcryptofile_writer_new.argtypes = [ c_int, c_int, GETHMACKEY, GETTRACEKEY ]
    lib.redcryptofile_writer_new.restype = c_void_p


# RedCryptoWriterHandle * redcryptofile_open(RedCryptoWriterHandle * handle, const char * path)
    lib.redcryptofile_writer_open.argtypes = [ c_void_p, c_char_p ]
    lib.redcryptofile_writer_open.restype = c_int

# int redcryptofile_write(RedCryptoWriterHandle * handle, uint8_t const * buffer, unsigned long len);
    lib.redcryptofile_writer_write.argtypes = [c_void_p, c_char_p, c_uint64 ]
    lib.redcryptofile_writer_write.restype = c_int

# int redcryptofile_close(RedCryptoWriterHandle * handle, HashHexArray qhashhex, HashHexArray fhashhex);
    lib.redcryptofile_writer_close.argtypes = [ c_void_p ]
    lib.redcryptofile_writer_close.restype = c_int

# const char * redcryptofile_qhashhex(RedCryptoWriterHandle * handle);
    lib.redcryptofile_writer_qhashhex.argtypes = [ c_void_p ]
    lib.redcryptofile_writer_qhashhex.restype = c_char_p
    
# const char * redcryptofile_fhashhex(RedCryptoWriterHandle * handle);
    lib.redcryptofile_writer_fhashhex.argtypes = [ c_void_p ]
    lib.redcryptofile_writer_fhashhex.restype = c_char_p

# void redcryptofile_delete(RedCryptoWriterHandle * handle);
    lib.redcryptofile_writer_delete.argtypes = [ c_void_p ]
    lib.redcryptofile_writer_delete.restype = None


except Exception as e:
    print("Failed to load redcryptofile library: %s\n" % str(e))
    sys.exit(10)
    
    
import unittest
    
class TestEncrypter(unittest.TestCase):
    def test(self):
        handle = lib.redcryptofile_writer_new(0, 0, get_hmac_key_func, get_trace_key_func)
        self.assertNotEqual(handle, None)
        lib.redcryptofile_writer_open(handle, "./clear.txt")
        
        text = b"We write, and again, and so on."
        total_sent = 0
        
        while total_sent < len(text):
            part_len = min(10,len(text[total_sent:]))
            res = lib.redcryptofile_writer_write(handle, text[total_sent:], part_len)
            self.assertTrue(res > 0)
            if res < 0: 
                break
            total_sent += res
        
        self.assertEqual(total_sent, 31)
        lib.redcryptofile_writer_close(handle)
        lib.redcryptofile_writer_delete(handle)


    def test_writer_checksum(self):
        handle = lib.redcryptofile_writer_new(0, 1, get_hmac_key_func, get_trace_key_func)
        self.assertNotEqual(handle, None)
        lib.redcryptofile_writer_open(handle, "./clear.txt")
        
        text = b"We write, and again, and so on."
        total_sent = 0
        
        while total_sent < len(text):
            part_len = min(10,len(text[total_sent:]))
            res = lib.redcryptofile_writer_write(handle, text[total_sent:], part_len)
            self.assertTrue(res > 0)
            if res < 0: 
                break
            total_sent += res
        
        self.assertEqual(total_sent, 31)
        lib.redcryptofile_writer_close(handle)

        self.assertEqual(lib.redcryptofile_writer_qhashhex(handle),         
            'E0901B761D62E8A6F41F729E3CBCF3F0AF4E0386046D45258DF50C06F16C6722')

        self.assertEqual(lib.redcryptofile_writer_fhashhex(handle),         
            'E0901B761D62E8A6F41F729E3CBCF3F0AF4E0386046D45258DF50C06F16C6722')

        lib.redcryptofile_writer_delete(handle)

    def test_writer_encryption_checksum(self):
        handle = lib.redcryptofile_writer_new(1, 1, get_hmac_key_func, get_trace_key_func)
        self.assertNotEqual(handle, None)
        lib.redcryptofile_writer_open(handle, "./encrypted.txt")
        
        text = b"We write, and again, and so on."
        total_sent = 0
        
        while total_sent < len(text):
            part_len = min(10,len(text[total_sent:]))
            res = lib.redcryptofile_writer_write(handle, text[total_sent:], part_len)
            self.assertTrue(res > 0)
            if res < 0: 
                break
            total_sent += res
        
        self.assertEqual(total_sent, 31)
        lib.redcryptofile_writer_close(handle)

        self.assertEqual(lib.redcryptofile_writer_qhashhex(handle),
            'CE901886A85C774C080921215D31A91D7D8E8AA14040D081E250EE53300CABF0')

        self.assertEqual(lib.redcryptofile_writer_fhashhex(handle),         
            'CE901886A85C774C080921215D31A91D7D8E8AA14040D081E250EE53300CABF0')

        lib.redcryptofile_writer_delete(handle)

if __name__ == '__main__':
    unittest.main()
        
