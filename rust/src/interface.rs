#![allow(unknown_lints)]
#![allow(mutex_atomic, needless_pass_by_value)]
use libc::{c_int, c_void, uint8_t, uint16_t};
use std::slice;

use std::sync::{Arc, Mutex};
use std::ptr::null;

use implementation::*;


#[repr(C)]
pub struct COption<T> {
    data: T,
    some: bool,
}

impl<T> From<Option<T>> for COption<T>
where
    T: Default,
{
    fn from(t: Option<T>) -> COption<T> {
        if let Some(v) = t {
            COption {
                data: v,
                some: true,
            }
        } else {
            COption {
                data: T::default(),
                some: false,
            }
        }
    }
}


#[repr(C)]
pub struct QString {
    data: *const uint8_t,
    len: c_int,
}

#[repr(C)]
pub struct QStringIn {
    data: *const uint16_t,
    len: c_int,
}

impl QStringIn {
    fn convert(&self) -> String {
        let data = unsafe { slice::from_raw_parts(self.data, self.len as usize) };
        String::from_utf16_lossy(data)
    }
}

impl<'a> From<&'a str> for QString {
    fn from(string: &'a str) -> QString {
        QString {
            len: string.len() as c_int,
            data: string.as_ptr(),
        }
    }
}

impl<'a> From<&'a String> for QString {
    fn from(string: &'a String) -> QString {
        QString {
            len: string.len() as c_int,
            data: string.as_ptr(),
        }
    }
}


#[repr(C)]
pub struct QByteArray {
    data: *const uint8_t,
    len: c_int,
}

impl QByteArray {
    fn convert(&self) -> Vec<u8> {
        let data = unsafe { slice::from_raw_parts(self.data, self.len as usize) };
        Vec::from(data)
    }
}

impl<'a> From<&'a [u8]> for QByteArray {
    fn from(value: &'a [u8]) -> QByteArray {
        QByteArray {
            len: value.len() as c_int,
            data: value.as_ptr(),
        }
    }
}


#[repr(C)]
pub enum SortOrder {
    Ascending = 0,
    Descending = 1,
}

#[repr(C)]
pub struct QModelIndex {
    row: c_int,
    internal_id: usize,
}

pub struct DemoQObject {}

#[derive(Clone)]
pub struct DemoEmitter {
    qobject: Arc<Mutex<*const DemoQObject>>,
}

unsafe impl Send for DemoEmitter {}

impl DemoEmitter {
    fn clear(&self) {
        *self.qobject.lock().unwrap() = null();
    }
}

pub trait DemoTrait {
    fn new(emit: DemoEmitter,
        fibonacci: Fibonacci,
        fibonacci_list: FibonacciList,
        file_system_tree: FileSystemTree,
        processes: Processes,
        time_series: TimeSeries) -> Self;
    fn emit(&self) -> &DemoEmitter;
    fn fibonacci(&self) -> &Fibonacci;
    fn fibonacci_mut(&mut self) -> &mut Fibonacci;
    fn fibonacci_list(&self) -> &FibonacciList;
    fn fibonacci_list_mut(&mut self) -> &mut FibonacciList;
    fn file_system_tree(&self) -> &FileSystemTree;
    fn file_system_tree_mut(&mut self) -> &mut FileSystemTree;
    fn processes(&self) -> &Processes;
    fn processes_mut(&mut self) -> &mut Processes;
    fn time_series(&self) -> &TimeSeries;
    fn time_series_mut(&mut self) -> &mut TimeSeries;
}

#[no_mangle]
pub extern "C" fn demo_new(
    demo: *mut DemoQObject,
    fibonacci: *mut FibonacciQObject,
    input_changed: fn(*const FibonacciQObject),
    result_changed: fn(*const FibonacciQObject),
    fibonacci_list: *mut FibonacciListQObject,
    fibonacci_list_new_data_ready: fn(*const FibonacciListQObject),
    fibonacci_list_data_changed: fn(*const FibonacciListQObject, usize, usize),
    fibonacci_list_begin_reset_model: fn(*const FibonacciListQObject),
    fibonacci_list_end_reset_model: fn(*const FibonacciListQObject),
    fibonacci_list_begin_insert_rows: fn(*const FibonacciListQObject, usize, usize),
    fibonacci_list_end_insert_rows: fn(*const FibonacciListQObject),
    fibonacci_list_begin_remove_rows: fn(*const FibonacciListQObject, usize, usize),
    fibonacci_list_end_remove_rows: fn(*const FibonacciListQObject),
    file_system_tree: *mut FileSystemTreeQObject,
    path_changed: fn(*const FileSystemTreeQObject),
    file_system_tree_new_data_ready: fn(*const FileSystemTreeQObject, item: usize, valid: bool),
    file_system_tree_data_changed: fn(*const FileSystemTreeQObject, usize, usize),
    file_system_tree_begin_reset_model: fn(*const FileSystemTreeQObject),
    file_system_tree_end_reset_model: fn(*const FileSystemTreeQObject),
    file_system_tree_begin_insert_rows: fn(*const FileSystemTreeQObject, item: usize, valid: bool, usize, usize),
    file_system_tree_end_insert_rows: fn(*const FileSystemTreeQObject),
    file_system_tree_begin_remove_rows: fn(*const FileSystemTreeQObject, item: usize, valid: bool, usize, usize),
    file_system_tree_end_remove_rows: fn(*const FileSystemTreeQObject),
    processes: *mut ProcessesQObject,
    active_changed: fn(*const ProcessesQObject),
    processes_new_data_ready: fn(*const ProcessesQObject, item: usize, valid: bool),
    processes_data_changed: fn(*const ProcessesQObject, usize, usize),
    processes_begin_reset_model: fn(*const ProcessesQObject),
    processes_end_reset_model: fn(*const ProcessesQObject),
    processes_begin_insert_rows: fn(*const ProcessesQObject, item: usize, valid: bool, usize, usize),
    processes_end_insert_rows: fn(*const ProcessesQObject),
    processes_begin_remove_rows: fn(*const ProcessesQObject, item: usize, valid: bool, usize, usize),
    processes_end_remove_rows: fn(*const ProcessesQObject),
    time_series: *mut TimeSeriesQObject,
    time_series_new_data_ready: fn(*const TimeSeriesQObject),
    time_series_data_changed: fn(*const TimeSeriesQObject, usize, usize),
    time_series_begin_reset_model: fn(*const TimeSeriesQObject),
    time_series_end_reset_model: fn(*const TimeSeriesQObject),
    time_series_begin_insert_rows: fn(*const TimeSeriesQObject, usize, usize),
    time_series_end_insert_rows: fn(*const TimeSeriesQObject),
    time_series_begin_remove_rows: fn(*const TimeSeriesQObject, usize, usize),
    time_series_end_remove_rows: fn(*const TimeSeriesQObject),
) -> *mut Demo {
    let fibonacci_emit = FibonacciEmitter {
        qobject: Arc::new(Mutex::new(fibonacci)),
        input_changed: input_changed,
        result_changed: result_changed,
    };
    let d_fibonacci = Fibonacci::new(fibonacci_emit);
    let fibonacci_list_emit = FibonacciListEmitter {
        qobject: Arc::new(Mutex::new(fibonacci_list)),
        new_data_ready: fibonacci_list_new_data_ready,
    };
    let model = FibonacciListList {
        qobject: fibonacci_list,
        data_changed: fibonacci_list_data_changed,
        begin_reset_model: fibonacci_list_begin_reset_model,
        end_reset_model: fibonacci_list_end_reset_model,
        begin_insert_rows: fibonacci_list_begin_insert_rows,
        end_insert_rows: fibonacci_list_end_insert_rows,
        begin_remove_rows: fibonacci_list_begin_remove_rows,
        end_remove_rows: fibonacci_list_end_remove_rows,
    };
    let d_fibonacci_list = FibonacciList::new(fibonacci_list_emit, model);
    let file_system_tree_emit = FileSystemTreeEmitter {
        qobject: Arc::new(Mutex::new(file_system_tree)),
        path_changed: path_changed,
        new_data_ready: file_system_tree_new_data_ready,
    };
    let model = FileSystemTreeTree {
        qobject: file_system_tree,
        data_changed: file_system_tree_data_changed,
        begin_reset_model: file_system_tree_begin_reset_model,
        end_reset_model: file_system_tree_end_reset_model,
        begin_insert_rows: file_system_tree_begin_insert_rows,
        end_insert_rows: file_system_tree_end_insert_rows,
        begin_remove_rows: file_system_tree_begin_remove_rows,
        end_remove_rows: file_system_tree_end_remove_rows,
    };
    let d_file_system_tree = FileSystemTree::new(file_system_tree_emit, model);
    let processes_emit = ProcessesEmitter {
        qobject: Arc::new(Mutex::new(processes)),
        active_changed: active_changed,
        new_data_ready: processes_new_data_ready,
    };
    let model = ProcessesTree {
        qobject: processes,
        data_changed: processes_data_changed,
        begin_reset_model: processes_begin_reset_model,
        end_reset_model: processes_end_reset_model,
        begin_insert_rows: processes_begin_insert_rows,
        end_insert_rows: processes_end_insert_rows,
        begin_remove_rows: processes_begin_remove_rows,
        end_remove_rows: processes_end_remove_rows,
    };
    let d_processes = Processes::new(processes_emit, model);
    let time_series_emit = TimeSeriesEmitter {
        qobject: Arc::new(Mutex::new(time_series)),
        new_data_ready: time_series_new_data_ready,
    };
    let model = TimeSeriesList {
        qobject: time_series,
        data_changed: time_series_data_changed,
        begin_reset_model: time_series_begin_reset_model,
        end_reset_model: time_series_end_reset_model,
        begin_insert_rows: time_series_begin_insert_rows,
        end_insert_rows: time_series_end_insert_rows,
        begin_remove_rows: time_series_begin_remove_rows,
        end_remove_rows: time_series_end_remove_rows,
    };
    let d_time_series = TimeSeries::new(time_series_emit, model);
    let demo_emit = DemoEmitter {
        qobject: Arc::new(Mutex::new(demo)),
    };
    let d_demo = Demo::new(demo_emit,
        d_fibonacci,
        d_fibonacci_list,
        d_file_system_tree,
        d_processes,
        d_time_series);
    Box::into_raw(Box::new(d_demo))
}

#[no_mangle]
pub unsafe extern "C" fn demo_free(ptr: *mut Demo) {
    Box::from_raw(ptr).emit().clear();
}

#[no_mangle]
pub unsafe extern "C" fn demo_fibonacci_get(ptr: *mut Demo) -> *mut Fibonacci {
    (&mut *ptr).fibonacci_mut()
}

#[no_mangle]
pub unsafe extern "C" fn demo_fibonacci_list_get(ptr: *mut Demo) -> *mut FibonacciList {
    (&mut *ptr).fibonacci_list_mut()
}

#[no_mangle]
pub unsafe extern "C" fn demo_file_system_tree_get(ptr: *mut Demo) -> *mut FileSystemTree {
    (&mut *ptr).file_system_tree_mut()
}

#[no_mangle]
pub unsafe extern "C" fn demo_processes_get(ptr: *mut Demo) -> *mut Processes {
    (&mut *ptr).processes_mut()
}

#[no_mangle]
pub unsafe extern "C" fn demo_time_series_get(ptr: *mut Demo) -> *mut TimeSeries {
    (&mut *ptr).time_series_mut()
}

pub struct FibonacciQObject {}

#[derive(Clone)]
pub struct FibonacciEmitter {
    qobject: Arc<Mutex<*const FibonacciQObject>>,
    input_changed: fn(*const FibonacciQObject),
    result_changed: fn(*const FibonacciQObject),
}

unsafe impl Send for FibonacciEmitter {}

impl FibonacciEmitter {
    fn clear(&self) {
        *self.qobject.lock().unwrap() = null();
    }
    pub fn input_changed(&self) {
        let ptr = *self.qobject.lock().unwrap();
        if !ptr.is_null() {
            (self.input_changed)(ptr);
        }
    }
    pub fn result_changed(&self) {
        let ptr = *self.qobject.lock().unwrap();
        if !ptr.is_null() {
            (self.result_changed)(ptr);
        }
    }
}

pub trait FibonacciTrait {
    fn new(emit: FibonacciEmitter) -> Self;
    fn emit(&self) -> &FibonacciEmitter;
    fn input(&self) -> u32;
    fn set_input(&mut self, value: u32);
    fn result(&self) -> u64;
}

#[no_mangle]
pub extern "C" fn fibonacci_new(
    fibonacci: *mut FibonacciQObject,
    input_changed: fn(*const FibonacciQObject),
    result_changed: fn(*const FibonacciQObject),
) -> *mut Fibonacci {
    let fibonacci_emit = FibonacciEmitter {
        qobject: Arc::new(Mutex::new(fibonacci)),
        input_changed: input_changed,
        result_changed: result_changed,
    };
    let d_fibonacci = Fibonacci::new(fibonacci_emit);
    Box::into_raw(Box::new(d_fibonacci))
}

#[no_mangle]
pub unsafe extern "C" fn fibonacci_free(ptr: *mut Fibonacci) {
    Box::from_raw(ptr).emit().clear();
}

#[no_mangle]
pub unsafe extern "C" fn fibonacci_input_get(ptr: *const Fibonacci) -> u32 {
    (&*ptr).input()
}

#[no_mangle]
pub unsafe extern "C" fn fibonacci_input_set(ptr: *mut Fibonacci, v: u32) {
    (&mut *ptr).set_input(v);
}

#[no_mangle]
pub unsafe extern "C" fn fibonacci_result_get(ptr: *const Fibonacci) -> u64 {
    (&*ptr).result()
}

pub struct FibonacciListQObject {}

#[derive(Clone)]
pub struct FibonacciListEmitter {
    qobject: Arc<Mutex<*const FibonacciListQObject>>,
    new_data_ready: fn(*const FibonacciListQObject),
}

unsafe impl Send for FibonacciListEmitter {}

impl FibonacciListEmitter {
    fn clear(&self) {
        *self.qobject.lock().unwrap() = null();
    }
    pub fn new_data_ready(&self) {
        let ptr = *self.qobject.lock().unwrap();
        if !ptr.is_null() {
            (self.new_data_ready)(ptr);
        }
    }
}

pub struct FibonacciListList {
    qobject: *const FibonacciListQObject,
    data_changed: fn(*const FibonacciListQObject, usize, usize),
    begin_reset_model: fn(*const FibonacciListQObject),
    end_reset_model: fn(*const FibonacciListQObject),
    begin_insert_rows: fn(*const FibonacciListQObject, usize, usize),
    end_insert_rows: fn(*const FibonacciListQObject),
    begin_remove_rows: fn(*const FibonacciListQObject, usize, usize),
    end_remove_rows: fn(*const FibonacciListQObject),
}

impl FibonacciListList {
    pub fn data_changed(&self, first: usize, last: usize) {
        (self.data_changed)(self.qobject, first, last);
    }
    pub fn begin_reset_model(&self) {
        (self.begin_reset_model)(self.qobject);
    }
    pub fn end_reset_model(&self) {
        (self.end_reset_model)(self.qobject);
    }
    pub fn begin_insert_rows(&self, first: usize, last: usize) {
        (self.begin_insert_rows)(self.qobject, first, last);
    }
    pub fn end_insert_rows(&self) {
        (self.end_insert_rows)(self.qobject);
    }
    pub fn begin_remove_rows(&self, first: usize, last: usize) {
        (self.begin_remove_rows)(self.qobject, first, last);
    }
    pub fn end_remove_rows(&self) {
        (self.end_remove_rows)(self.qobject);
    }
}

pub trait FibonacciListTrait {
    fn new(emit: FibonacciListEmitter, model: FibonacciListList) -> Self;
    fn emit(&self) -> &FibonacciListEmitter;
    fn row_count(&self) -> usize;
    fn insert_rows(&mut self, row: usize, count: usize) -> bool { false }
    fn remove_rows(&mut self, row: usize, count: usize) -> bool { false }
    fn can_fetch_more(&self) -> bool {
        false
    }
    fn fetch_more(&mut self) {}
    fn sort(&mut self, u8, SortOrder) {}
    fn fibonacci_number(&self, item: usize) -> u64;
    fn row(&self, item: usize) -> u64;
}

#[no_mangle]
pub extern "C" fn fibonacci_list_new(
    fibonacci_list: *mut FibonacciListQObject,
    fibonacci_list_new_data_ready: fn(*const FibonacciListQObject),
    fibonacci_list_data_changed: fn(*const FibonacciListQObject, usize, usize),
    fibonacci_list_begin_reset_model: fn(*const FibonacciListQObject),
    fibonacci_list_end_reset_model: fn(*const FibonacciListQObject),
    fibonacci_list_begin_insert_rows: fn(*const FibonacciListQObject, usize, usize),
    fibonacci_list_end_insert_rows: fn(*const FibonacciListQObject),
    fibonacci_list_begin_remove_rows: fn(*const FibonacciListQObject, usize, usize),
    fibonacci_list_end_remove_rows: fn(*const FibonacciListQObject),
) -> *mut FibonacciList {
    let fibonacci_list_emit = FibonacciListEmitter {
        qobject: Arc::new(Mutex::new(fibonacci_list)),
        new_data_ready: fibonacci_list_new_data_ready,
    };
    let model = FibonacciListList {
        qobject: fibonacci_list,
        data_changed: fibonacci_list_data_changed,
        begin_reset_model: fibonacci_list_begin_reset_model,
        end_reset_model: fibonacci_list_end_reset_model,
        begin_insert_rows: fibonacci_list_begin_insert_rows,
        end_insert_rows: fibonacci_list_end_insert_rows,
        begin_remove_rows: fibonacci_list_begin_remove_rows,
        end_remove_rows: fibonacci_list_end_remove_rows,
    };
    let d_fibonacci_list = FibonacciList::new(fibonacci_list_emit, model);
    Box::into_raw(Box::new(d_fibonacci_list))
}

#[no_mangle]
pub unsafe extern "C" fn fibonacci_list_free(ptr: *mut FibonacciList) {
    Box::from_raw(ptr).emit().clear();
}

#[no_mangle]
pub unsafe extern "C" fn fibonacci_list_row_count(ptr: *const FibonacciList) -> c_int {
    (&*ptr).row_count() as c_int
}
#[no_mangle]
pub unsafe extern "C" fn fibonacci_list_insert_rows(ptr: *mut FibonacciList, row: c_int, count: c_int) -> bool {
    (&mut *ptr).insert_rows(row as usize, count as usize)
}
#[no_mangle]
pub unsafe extern "C" fn fibonacci_list_remove_rows(ptr: *mut FibonacciList, row: c_int, count: c_int) -> bool {
    (&mut *ptr).remove_rows(row as usize, count as usize)
}
#[no_mangle]
pub unsafe extern "C" fn fibonacci_list_can_fetch_more(ptr: *const FibonacciList) -> bool {
    (&*ptr).can_fetch_more()
}
#[no_mangle]
pub unsafe extern "C" fn fibonacci_list_fetch_more(ptr: *mut FibonacciList) {
    (&mut *ptr).fetch_more()
}
#[no_mangle]
pub unsafe extern "C" fn fibonacci_list_sort(
    ptr: *mut FibonacciList,
    column: u8,
    order: SortOrder,
) {
    (&mut *ptr).sort(column, order)
}

#[no_mangle]
pub unsafe extern "C" fn fibonacci_list_data_fibonacci_number(ptr: *const FibonacciList, row: c_int) -> u64 {
    (&*ptr).fibonacci_number(row as usize).into()
}

#[no_mangle]
pub unsafe extern "C" fn fibonacci_list_data_row(ptr: *const FibonacciList, row: c_int) -> u64 {
    (&*ptr).row(row as usize).into()
}

pub struct FileSystemTreeQObject {}

#[derive(Clone)]
pub struct FileSystemTreeEmitter {
    qobject: Arc<Mutex<*const FileSystemTreeQObject>>,
    path_changed: fn(*const FileSystemTreeQObject),
    new_data_ready: fn(*const FileSystemTreeQObject, item: usize, valid: bool),
}

unsafe impl Send for FileSystemTreeEmitter {}

impl FileSystemTreeEmitter {
    fn clear(&self) {
        *self.qobject.lock().unwrap() = null();
    }
    pub fn path_changed(&self) {
        let ptr = *self.qobject.lock().unwrap();
        if !ptr.is_null() {
            (self.path_changed)(ptr);
        }
    }
    pub fn new_data_ready(&self, item: Option<usize>) {
        let ptr = *self.qobject.lock().unwrap();
        if !ptr.is_null() {
            (self.new_data_ready)(ptr, item.unwrap_or(13), item.is_some());
        }
    }
}

pub struct FileSystemTreeTree {
    qobject: *const FileSystemTreeQObject,
    data_changed: fn(*const FileSystemTreeQObject, usize, usize),
    begin_reset_model: fn(*const FileSystemTreeQObject),
    end_reset_model: fn(*const FileSystemTreeQObject),
    begin_insert_rows: fn(*const FileSystemTreeQObject, item: usize, valid: bool, usize, usize),
    end_insert_rows: fn(*const FileSystemTreeQObject),
    begin_remove_rows: fn(*const FileSystemTreeQObject, item: usize, valid: bool, usize, usize),
    end_remove_rows: fn(*const FileSystemTreeQObject),
}

impl FileSystemTreeTree {
    pub fn data_changed(&self, first: usize, last: usize) {
        (self.data_changed)(self.qobject, first, last);
    }
    pub fn begin_reset_model(&self) {
        (self.begin_reset_model)(self.qobject);
    }
    pub fn end_reset_model(&self) {
        (self.end_reset_model)(self.qobject);
    }
    pub fn begin_insert_rows(&self, item: Option<usize>, first: usize, last: usize) {
        (self.begin_insert_rows)(self.qobject, item.unwrap_or(13), item.is_some(), first, last);
    }
    pub fn end_insert_rows(&self) {
        (self.end_insert_rows)(self.qobject);
    }
    pub fn begin_remove_rows(&self, item: Option<usize>, first: usize, last: usize) {
        (self.begin_remove_rows)(self.qobject, item.unwrap_or(13), item.is_some(), first, last);
    }
    pub fn end_remove_rows(&self) {
        (self.end_remove_rows)(self.qobject);
    }
}

pub trait FileSystemTreeTrait {
    fn new(emit: FileSystemTreeEmitter, model: FileSystemTreeTree) -> Self;
    fn emit(&self) -> &FileSystemTreeEmitter;
    fn path(&self) -> Option<&str>;
    fn set_path(&mut self, value: Option<String>);
    fn row_count(&self, Option<usize>) -> usize;
    fn can_fetch_more(&self, Option<usize>) -> bool {
        false
    }
    fn fetch_more(&mut self, Option<usize>) {}
    fn sort(&mut self, u8, SortOrder) {}
    fn index(&self, item: Option<usize>, row: usize) -> usize;
    fn parent(&self, item: usize) -> Option<usize>;
    fn row(&self, item: usize) -> usize;
    fn file_icon(&self, item: usize) -> &[u8];
    fn file_name(&self, item: usize) -> String;
    fn file_path(&self, item: usize) -> Option<String>;
    fn file_permissions(&self, item: usize) -> i32;
    fn file_size(&self, item: usize) -> Option<u64>;
    fn file_type(&self, item: usize) -> i32;
}

#[no_mangle]
pub extern "C" fn file_system_tree_new(
    file_system_tree: *mut FileSystemTreeQObject,
    path_changed: fn(*const FileSystemTreeQObject),
    file_system_tree_new_data_ready: fn(*const FileSystemTreeQObject, item: usize, valid: bool),
    file_system_tree_data_changed: fn(*const FileSystemTreeQObject, usize, usize),
    file_system_tree_begin_reset_model: fn(*const FileSystemTreeQObject),
    file_system_tree_end_reset_model: fn(*const FileSystemTreeQObject),
    file_system_tree_begin_insert_rows: fn(*const FileSystemTreeQObject, item: usize, valid: bool, usize, usize),
    file_system_tree_end_insert_rows: fn(*const FileSystemTreeQObject),
    file_system_tree_begin_remove_rows: fn(*const FileSystemTreeQObject, item: usize, valid: bool, usize, usize),
    file_system_tree_end_remove_rows: fn(*const FileSystemTreeQObject),
) -> *mut FileSystemTree {
    let file_system_tree_emit = FileSystemTreeEmitter {
        qobject: Arc::new(Mutex::new(file_system_tree)),
        path_changed: path_changed,
        new_data_ready: file_system_tree_new_data_ready,
    };
    let model = FileSystemTreeTree {
        qobject: file_system_tree,
        data_changed: file_system_tree_data_changed,
        begin_reset_model: file_system_tree_begin_reset_model,
        end_reset_model: file_system_tree_end_reset_model,
        begin_insert_rows: file_system_tree_begin_insert_rows,
        end_insert_rows: file_system_tree_end_insert_rows,
        begin_remove_rows: file_system_tree_begin_remove_rows,
        end_remove_rows: file_system_tree_end_remove_rows,
    };
    let d_file_system_tree = FileSystemTree::new(file_system_tree_emit, model);
    Box::into_raw(Box::new(d_file_system_tree))
}

#[no_mangle]
pub unsafe extern "C" fn file_system_tree_free(ptr: *mut FileSystemTree) {
    Box::from_raw(ptr).emit().clear();
}

#[no_mangle]
pub unsafe extern "C" fn file_system_tree_path_get(
    ptr: *const FileSystemTree,
    p: *mut c_void,
    set: fn(*mut c_void, QString),
) {
    let data = (&*ptr).path();
    if let Some(data) = data {
        set(p, data.into());
    }
}

#[no_mangle]
pub unsafe extern "C" fn file_system_tree_path_set(ptr: *mut FileSystemTree, v: QStringIn) {
    (&mut *ptr).set_path(Some(v.convert()));
}
#[no_mangle]
pub unsafe extern "C" fn file_system_tree_path_set_none(ptr: *mut FileSystemTree) {
    (&mut *ptr).set_path(None);
}

#[no_mangle]
pub unsafe extern "C" fn file_system_tree_row_count(
    ptr: *const FileSystemTree,
    item: usize,
    valid: bool,
) -> c_int {
    if valid {
        (&*ptr).row_count(Some(item)) as c_int
    } else {
        (&*ptr).row_count(None) as c_int
    }
}
#[no_mangle]
pub unsafe extern "C" fn file_system_tree_can_fetch_more(
    ptr: *const FileSystemTree,
    item: usize,
    valid: bool,
) -> bool {
    if valid {
        (&*ptr).can_fetch_more(Some(item))
    } else {
        (&*ptr).can_fetch_more(None)
    }
}
#[no_mangle]
pub unsafe extern "C" fn file_system_tree_fetch_more(ptr: *mut FileSystemTree, item: usize, valid: bool) {
    if valid {
        (&mut *ptr).fetch_more(Some(item))
    } else {
        (&mut *ptr).fetch_more(None)
    }
}
#[no_mangle]
pub unsafe extern "C" fn file_system_tree_sort(
    ptr: *mut FileSystemTree,
    column: u8,
    order: SortOrder
) {
    (&mut *ptr).sort(column, order)
}
#[no_mangle]
pub unsafe extern "C" fn file_system_tree_index(
    ptr: *const FileSystemTree,
    item: usize,
    valid: bool,
    row: c_int,
) -> usize {
    if !valid {
        (&*ptr).index(None, row as usize)
    } else {
        (&*ptr).index(Some(item), row as usize)
    }
}
#[no_mangle]
pub unsafe extern "C" fn file_system_tree_parent(ptr: *const FileSystemTree, index: usize) -> QModelIndex {
    if let Some(parent) = (&*ptr).parent(index) {
        QModelIndex {
            row: (&*ptr).row(parent) as c_int,
            internal_id: parent,
        }
    } else {
        QModelIndex {
            row: -1,
            internal_id: 0,
        }
    }
}
#[no_mangle]
pub unsafe extern "C" fn file_system_tree_row(ptr: *const FileSystemTree, item: usize) -> c_int {
    (&*ptr).row(item) as c_int
}

#[no_mangle]
pub unsafe extern "C" fn file_system_tree_data_file_icon(
    ptr: *const FileSystemTree, item: usize,
    d: *mut c_void,
    set: fn(*mut c_void, QByteArray),
) {
    let data = (&*ptr).file_icon(item);
    set(d, (data).into());
}

#[no_mangle]
pub unsafe extern "C" fn file_system_tree_data_file_name(
    ptr: *const FileSystemTree, item: usize,
    d: *mut c_void,
    set: fn(*mut c_void, QString),
) {
    let data = (&*ptr).file_name(item);
    set(d, (&data).into());
}

#[no_mangle]
pub unsafe extern "C" fn file_system_tree_data_file_path(
    ptr: *const FileSystemTree, item: usize,
    d: *mut c_void,
    set: fn(*mut c_void, QString),
) {
    let data = (&*ptr).file_path(item);
    if let Some(data) = data {
        set(d, QString::from(&data));
    }
}

#[no_mangle]
pub unsafe extern "C" fn file_system_tree_data_file_permissions(ptr: *const FileSystemTree, item: usize) -> i32 {
    (&*ptr).file_permissions(item).into()
}

#[no_mangle]
pub unsafe extern "C" fn file_system_tree_data_file_size(ptr: *const FileSystemTree, item: usize) -> COption<u64> {
    (&*ptr).file_size(item).into()
}

#[no_mangle]
pub unsafe extern "C" fn file_system_tree_data_file_type(ptr: *const FileSystemTree, item: usize) -> i32 {
    (&*ptr).file_type(item).into()
}

pub struct ProcessesQObject {}

#[derive(Clone)]
pub struct ProcessesEmitter {
    qobject: Arc<Mutex<*const ProcessesQObject>>,
    active_changed: fn(*const ProcessesQObject),
    new_data_ready: fn(*const ProcessesQObject, item: usize, valid: bool),
}

unsafe impl Send for ProcessesEmitter {}

impl ProcessesEmitter {
    fn clear(&self) {
        *self.qobject.lock().unwrap() = null();
    }
    pub fn active_changed(&self) {
        let ptr = *self.qobject.lock().unwrap();
        if !ptr.is_null() {
            (self.active_changed)(ptr);
        }
    }
    pub fn new_data_ready(&self, item: Option<usize>) {
        let ptr = *self.qobject.lock().unwrap();
        if !ptr.is_null() {
            (self.new_data_ready)(ptr, item.unwrap_or(13), item.is_some());
        }
    }
}

pub struct ProcessesTree {
    qobject: *const ProcessesQObject,
    data_changed: fn(*const ProcessesQObject, usize, usize),
    begin_reset_model: fn(*const ProcessesQObject),
    end_reset_model: fn(*const ProcessesQObject),
    begin_insert_rows: fn(*const ProcessesQObject, item: usize, valid: bool, usize, usize),
    end_insert_rows: fn(*const ProcessesQObject),
    begin_remove_rows: fn(*const ProcessesQObject, item: usize, valid: bool, usize, usize),
    end_remove_rows: fn(*const ProcessesQObject),
}

impl ProcessesTree {
    pub fn data_changed(&self, first: usize, last: usize) {
        (self.data_changed)(self.qobject, first, last);
    }
    pub fn begin_reset_model(&self) {
        (self.begin_reset_model)(self.qobject);
    }
    pub fn end_reset_model(&self) {
        (self.end_reset_model)(self.qobject);
    }
    pub fn begin_insert_rows(&self, item: Option<usize>, first: usize, last: usize) {
        (self.begin_insert_rows)(self.qobject, item.unwrap_or(13), item.is_some(), first, last);
    }
    pub fn end_insert_rows(&self) {
        (self.end_insert_rows)(self.qobject);
    }
    pub fn begin_remove_rows(&self, item: Option<usize>, first: usize, last: usize) {
        (self.begin_remove_rows)(self.qobject, item.unwrap_or(13), item.is_some(), first, last);
    }
    pub fn end_remove_rows(&self) {
        (self.end_remove_rows)(self.qobject);
    }
}

pub trait ProcessesTrait {
    fn new(emit: ProcessesEmitter, model: ProcessesTree) -> Self;
    fn emit(&self) -> &ProcessesEmitter;
    fn active(&self) -> bool;
    fn set_active(&mut self, value: bool);
    fn row_count(&self, Option<usize>) -> usize;
    fn can_fetch_more(&self, Option<usize>) -> bool {
        false
    }
    fn fetch_more(&mut self, Option<usize>) {}
    fn sort(&mut self, u8, SortOrder) {}
    fn index(&self, item: Option<usize>, row: usize) -> usize;
    fn parent(&self, item: usize) -> Option<usize>;
    fn row(&self, item: usize) -> usize;
    fn cmd(&self, item: usize) -> String;
    fn cpu_percentage(&self, item: usize) -> u8;
    fn cpu_usage(&self, item: usize) -> f32;
    fn memory(&self, item: usize) -> u64;
    fn name(&self, item: usize) -> &str;
    fn pid(&self, item: usize) -> u32;
    fn uid(&self, item: usize) -> u32;
}

#[no_mangle]
pub extern "C" fn processes_new(
    processes: *mut ProcessesQObject,
    active_changed: fn(*const ProcessesQObject),
    processes_new_data_ready: fn(*const ProcessesQObject, item: usize, valid: bool),
    processes_data_changed: fn(*const ProcessesQObject, usize, usize),
    processes_begin_reset_model: fn(*const ProcessesQObject),
    processes_end_reset_model: fn(*const ProcessesQObject),
    processes_begin_insert_rows: fn(*const ProcessesQObject, item: usize, valid: bool, usize, usize),
    processes_end_insert_rows: fn(*const ProcessesQObject),
    processes_begin_remove_rows: fn(*const ProcessesQObject, item: usize, valid: bool, usize, usize),
    processes_end_remove_rows: fn(*const ProcessesQObject),
) -> *mut Processes {
    let processes_emit = ProcessesEmitter {
        qobject: Arc::new(Mutex::new(processes)),
        active_changed: active_changed,
        new_data_ready: processes_new_data_ready,
    };
    let model = ProcessesTree {
        qobject: processes,
        data_changed: processes_data_changed,
        begin_reset_model: processes_begin_reset_model,
        end_reset_model: processes_end_reset_model,
        begin_insert_rows: processes_begin_insert_rows,
        end_insert_rows: processes_end_insert_rows,
        begin_remove_rows: processes_begin_remove_rows,
        end_remove_rows: processes_end_remove_rows,
    };
    let d_processes = Processes::new(processes_emit, model);
    Box::into_raw(Box::new(d_processes))
}

#[no_mangle]
pub unsafe extern "C" fn processes_free(ptr: *mut Processes) {
    Box::from_raw(ptr).emit().clear();
}

#[no_mangle]
pub unsafe extern "C" fn processes_active_get(ptr: *const Processes) -> bool {
    (&*ptr).active()
}

#[no_mangle]
pub unsafe extern "C" fn processes_active_set(ptr: *mut Processes, v: bool) {
    (&mut *ptr).set_active(v);
}

#[no_mangle]
pub unsafe extern "C" fn processes_row_count(
    ptr: *const Processes,
    item: usize,
    valid: bool,
) -> c_int {
    if valid {
        (&*ptr).row_count(Some(item)) as c_int
    } else {
        (&*ptr).row_count(None) as c_int
    }
}
#[no_mangle]
pub unsafe extern "C" fn processes_can_fetch_more(
    ptr: *const Processes,
    item: usize,
    valid: bool,
) -> bool {
    if valid {
        (&*ptr).can_fetch_more(Some(item))
    } else {
        (&*ptr).can_fetch_more(None)
    }
}
#[no_mangle]
pub unsafe extern "C" fn processes_fetch_more(ptr: *mut Processes, item: usize, valid: bool) {
    if valid {
        (&mut *ptr).fetch_more(Some(item))
    } else {
        (&mut *ptr).fetch_more(None)
    }
}
#[no_mangle]
pub unsafe extern "C" fn processes_sort(
    ptr: *mut Processes,
    column: u8,
    order: SortOrder
) {
    (&mut *ptr).sort(column, order)
}
#[no_mangle]
pub unsafe extern "C" fn processes_index(
    ptr: *const Processes,
    item: usize,
    valid: bool,
    row: c_int,
) -> usize {
    if !valid {
        (&*ptr).index(None, row as usize)
    } else {
        (&*ptr).index(Some(item), row as usize)
    }
}
#[no_mangle]
pub unsafe extern "C" fn processes_parent(ptr: *const Processes, index: usize) -> QModelIndex {
    if let Some(parent) = (&*ptr).parent(index) {
        QModelIndex {
            row: (&*ptr).row(parent) as c_int,
            internal_id: parent,
        }
    } else {
        QModelIndex {
            row: -1,
            internal_id: 0,
        }
    }
}
#[no_mangle]
pub unsafe extern "C" fn processes_row(ptr: *const Processes, item: usize) -> c_int {
    (&*ptr).row(item) as c_int
}

#[no_mangle]
pub unsafe extern "C" fn processes_data_cmd(
    ptr: *const Processes, item: usize,
    d: *mut c_void,
    set: fn(*mut c_void, QString),
) {
    let data = (&*ptr).cmd(item);
    set(d, (&data).into());
}

#[no_mangle]
pub unsafe extern "C" fn processes_data_cpu_percentage(ptr: *const Processes, item: usize) -> u8 {
    (&*ptr).cpu_percentage(item).into()
}

#[no_mangle]
pub unsafe extern "C" fn processes_data_cpu_usage(ptr: *const Processes, item: usize) -> f32 {
    (&*ptr).cpu_usage(item).into()
}

#[no_mangle]
pub unsafe extern "C" fn processes_data_memory(ptr: *const Processes, item: usize) -> u64 {
    (&*ptr).memory(item).into()
}

#[no_mangle]
pub unsafe extern "C" fn processes_data_name(
    ptr: *const Processes, item: usize,
    d: *mut c_void,
    set: fn(*mut c_void, QString),
) {
    let data = (&*ptr).name(item);
    set(d, (data).into());
}

#[no_mangle]
pub unsafe extern "C" fn processes_data_pid(ptr: *const Processes, item: usize) -> u32 {
    (&*ptr).pid(item).into()
}

#[no_mangle]
pub unsafe extern "C" fn processes_data_uid(ptr: *const Processes, item: usize) -> u32 {
    (&*ptr).uid(item).into()
}

pub struct TimeSeriesQObject {}

#[derive(Clone)]
pub struct TimeSeriesEmitter {
    qobject: Arc<Mutex<*const TimeSeriesQObject>>,
    new_data_ready: fn(*const TimeSeriesQObject),
}

unsafe impl Send for TimeSeriesEmitter {}

impl TimeSeriesEmitter {
    fn clear(&self) {
        *self.qobject.lock().unwrap() = null();
    }
    pub fn new_data_ready(&self) {
        let ptr = *self.qobject.lock().unwrap();
        if !ptr.is_null() {
            (self.new_data_ready)(ptr);
        }
    }
}

pub struct TimeSeriesList {
    qobject: *const TimeSeriesQObject,
    data_changed: fn(*const TimeSeriesQObject, usize, usize),
    begin_reset_model: fn(*const TimeSeriesQObject),
    end_reset_model: fn(*const TimeSeriesQObject),
    begin_insert_rows: fn(*const TimeSeriesQObject, usize, usize),
    end_insert_rows: fn(*const TimeSeriesQObject),
    begin_remove_rows: fn(*const TimeSeriesQObject, usize, usize),
    end_remove_rows: fn(*const TimeSeriesQObject),
}

impl TimeSeriesList {
    pub fn data_changed(&self, first: usize, last: usize) {
        (self.data_changed)(self.qobject, first, last);
    }
    pub fn begin_reset_model(&self) {
        (self.begin_reset_model)(self.qobject);
    }
    pub fn end_reset_model(&self) {
        (self.end_reset_model)(self.qobject);
    }
    pub fn begin_insert_rows(&self, first: usize, last: usize) {
        (self.begin_insert_rows)(self.qobject, first, last);
    }
    pub fn end_insert_rows(&self) {
        (self.end_insert_rows)(self.qobject);
    }
    pub fn begin_remove_rows(&self, first: usize, last: usize) {
        (self.begin_remove_rows)(self.qobject, first, last);
    }
    pub fn end_remove_rows(&self) {
        (self.end_remove_rows)(self.qobject);
    }
}

pub trait TimeSeriesTrait {
    fn new(emit: TimeSeriesEmitter, model: TimeSeriesList) -> Self;
    fn emit(&self) -> &TimeSeriesEmitter;
    fn row_count(&self) -> usize;
    fn insert_rows(&mut self, row: usize, count: usize) -> bool { false }
    fn remove_rows(&mut self, row: usize, count: usize) -> bool { false }
    fn can_fetch_more(&self) -> bool {
        false
    }
    fn fetch_more(&mut self) {}
    fn sort(&mut self, u8, SortOrder) {}
    fn cos(&self, item: usize) -> f32;
    fn set_cos(&mut self, item: usize, f32) -> bool;
    fn sin(&self, item: usize) -> f32;
    fn set_sin(&mut self, item: usize, f32) -> bool;
    fn time(&self, item: usize) -> f32;
    fn set_time(&mut self, item: usize, f32) -> bool;
}

#[no_mangle]
pub extern "C" fn time_series_new(
    time_series: *mut TimeSeriesQObject,
    time_series_new_data_ready: fn(*const TimeSeriesQObject),
    time_series_data_changed: fn(*const TimeSeriesQObject, usize, usize),
    time_series_begin_reset_model: fn(*const TimeSeriesQObject),
    time_series_end_reset_model: fn(*const TimeSeriesQObject),
    time_series_begin_insert_rows: fn(*const TimeSeriesQObject, usize, usize),
    time_series_end_insert_rows: fn(*const TimeSeriesQObject),
    time_series_begin_remove_rows: fn(*const TimeSeriesQObject, usize, usize),
    time_series_end_remove_rows: fn(*const TimeSeriesQObject),
) -> *mut TimeSeries {
    let time_series_emit = TimeSeriesEmitter {
        qobject: Arc::new(Mutex::new(time_series)),
        new_data_ready: time_series_new_data_ready,
    };
    let model = TimeSeriesList {
        qobject: time_series,
        data_changed: time_series_data_changed,
        begin_reset_model: time_series_begin_reset_model,
        end_reset_model: time_series_end_reset_model,
        begin_insert_rows: time_series_begin_insert_rows,
        end_insert_rows: time_series_end_insert_rows,
        begin_remove_rows: time_series_begin_remove_rows,
        end_remove_rows: time_series_end_remove_rows,
    };
    let d_time_series = TimeSeries::new(time_series_emit, model);
    Box::into_raw(Box::new(d_time_series))
}

#[no_mangle]
pub unsafe extern "C" fn time_series_free(ptr: *mut TimeSeries) {
    Box::from_raw(ptr).emit().clear();
}

#[no_mangle]
pub unsafe extern "C" fn time_series_row_count(ptr: *const TimeSeries) -> c_int {
    (&*ptr).row_count() as c_int
}
#[no_mangle]
pub unsafe extern "C" fn time_series_insert_rows(ptr: *mut TimeSeries, row: c_int, count: c_int) -> bool {
    (&mut *ptr).insert_rows(row as usize, count as usize)
}
#[no_mangle]
pub unsafe extern "C" fn time_series_remove_rows(ptr: *mut TimeSeries, row: c_int, count: c_int) -> bool {
    (&mut *ptr).remove_rows(row as usize, count as usize)
}
#[no_mangle]
pub unsafe extern "C" fn time_series_can_fetch_more(ptr: *const TimeSeries) -> bool {
    (&*ptr).can_fetch_more()
}
#[no_mangle]
pub unsafe extern "C" fn time_series_fetch_more(ptr: *mut TimeSeries) {
    (&mut *ptr).fetch_more()
}
#[no_mangle]
pub unsafe extern "C" fn time_series_sort(
    ptr: *mut TimeSeries,
    column: u8,
    order: SortOrder,
) {
    (&mut *ptr).sort(column, order)
}

#[no_mangle]
pub unsafe extern "C" fn time_series_data_cos(ptr: *const TimeSeries, row: c_int) -> f32 {
    (&*ptr).cos(row as usize).into()
}

#[no_mangle]
pub unsafe extern "C" fn time_series_set_data_cos(
    ptr: *mut TimeSeries, row: c_int,
    v: f32,
) -> bool {
    (&mut *ptr).set_cos(row as usize, v)
}

#[no_mangle]
pub unsafe extern "C" fn time_series_data_sin(ptr: *const TimeSeries, row: c_int) -> f32 {
    (&*ptr).sin(row as usize).into()
}

#[no_mangle]
pub unsafe extern "C" fn time_series_set_data_sin(
    ptr: *mut TimeSeries, row: c_int,
    v: f32,
) -> bool {
    (&mut *ptr).set_sin(row as usize, v)
}

#[no_mangle]
pub unsafe extern "C" fn time_series_data_time(ptr: *const TimeSeries, row: c_int) -> f32 {
    (&*ptr).time(row as usize).into()
}

#[no_mangle]
pub unsafe extern "C" fn time_series_set_data_time(
    ptr: *mut TimeSeries, row: c_int,
    v: f32,
) -> bool {
    (&mut *ptr).set_time(row as usize, v)
}
