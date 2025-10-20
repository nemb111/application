#[macro_export]
macro_rules! replace_expr {
    ($_t:tt $sub:expr) => {
        $sub
    };
}

#[macro_export]
macro_rules! count_tts {
    ($($tts:tt)*) => {<[()]>::len(&[$($crate::replace_expr!($tts ())),*])};
}

#[macro_export]
macro_rules! hashmap {
    ($($key:expr => $value:expr $(,)?)* ) => {
       {
           let mut hashMap = ::std::collections::HashMap::with_capacity($crate::count_tts!($($key)*));
           $(hashMap.insert($key, $value);)*

           hashMap
       }
    };
}
