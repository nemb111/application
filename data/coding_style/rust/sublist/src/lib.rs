#[derive(Debug, PartialEq)]
pub enum Comparison {
    Equal,
    Sublist,
    Superlist,
    Unequal,
}

pub fn sublist<T: PartialEq>(first_list: &[T], second_list: &[T]) -> Comparison {
    use std::cmp::Ordering;
    use Comparison::{Equal, Sublist, Superlist};

    let (eligible_comp, smaller_slice, bigger_slice, smaller_slice_size) =
        match first_list.len().cmp(&second_list.len()) {
            Ordering::Equal => (Equal, first_list, second_list, first_list.len()),
            Ordering::Less => (Sublist, first_list, second_list, first_list.len()),
            Ordering::Greater => (Superlist, second_list, first_list, second_list.len()),
        };

    // If one of the slices is empty the other slice is sub or super list
    if 0 == smaller_slice_size {
        return eligible_comp;
    }

    for win in bigger_slice.windows(smaller_slice_size) {
        if win.eq(smaller_slice) {
            return eligible_comp;
        }
    }

    Comparison::Unequal
}
