/// What should the type of function be?
pub fn map<C, T, U>(input: Vec<T>, mut function: C) -> Vec<U>
where
    C: FnMut(T) -> U,
{
    let mut res = Vec::with_capacity(input.len());

    for i in input.into_iter() {
        res.push(function(i));
    }

    res
}
