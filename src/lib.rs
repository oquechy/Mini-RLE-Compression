use std::slice;

extern crate libc;
use libc::{size_t, c_int, uint8_t};

#[no_mangle]
pub extern fn rle_encode(src: *const uint8_t, src_len: size_t, dst: *mut uint8_t, dst_len: size_t, bytes_written: &mut size_t) -> c_int {
    let mut isrc;
    let mut idst;
    unsafe {
        isrc = slice::from_raw_parts(src, src_len).iter(); 
        idst = slice::from_raw_parts_mut(dst, dst_len).iter_mut();
    }
    
    let (mut s, mut d) = (isrc.next(), idst.next());
    
    *bytes_written = 0;
    
    while s != None {
    // Need at least two bytes to write the next block
        if dst_len - *bytes_written < 2 { 
            return -1;
        }

        let (mut run, val) = (0, *s.unwrap());
        while s != None && *s.unwrap() == val && run < 255 {
            s = isrc.next();
            run += 1;
        }

        *d.unwrap() = run;
        d = idst.next();
        *d.unwrap() = val;
        d = idst.next();
        *bytes_written += 2;
    }
    0
}

#[no_mangle]
pub extern fn rle_decode(src: *const uint8_t, src_len: size_t, dst: *mut uint8_t, dst_len: size_t, bytes_written: &mut size_t) -> c_int {
    // Need an even number of bytes to decode
    if (src_len & 1) != 0 {
        return -1;
    }
    
    let mut isrc;
    let mut idst;
    unsafe {
        isrc = slice::from_raw_parts(src, src_len).iter(); 
        idst = slice::from_raw_parts_mut(dst, dst_len).iter_mut();
    }
    
    let (mut s, mut d) = (isrc.next(), idst.next());

    *bytes_written = 0;
    
    while s != None {
        let run = *s.unwrap() as size_t;
        s = isrc.next();
        let val = *s.unwrap();
        s = isrc.next();
        
        if dst_len - *bytes_written < run {
            return -1;
        }
        
        for _ in 0..run {
            *d.unwrap() = val;
            d = idst.next();
        } 
        *bytes_written += run;
    }
    0
} 

