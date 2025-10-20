use std::collections::HashMap;
use std::sync::atomic::AtomicUsize;

/// `InputCellId` is a unique identifier for an input cell.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct InputCellId(usize);
/// `ComputeCellId` is a unique identifier for a compute cell.
/// Values of type `InputCellId` and `ComputeCellId` should not be mutually assignable,
/// demonstrated by the following tests:
///
/// ```compile_fail
/// let mut r = react::Reactor::new();
/// let input: react::ComputeCellId = r.create_input(111);
/// ```
///
/// ```compile_fail
/// let mut r = react::Reactor::new();
/// let input = r.create_input(111);
/// let compute: react::InputCellId = r.create_compute(&[react::CellId::Input(input)], |_| 222).unwrap();
/// ```
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct ComputeCellId(usize);
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct CallbackId(usize);

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum CellId {
    Input(InputCellId),
    Compute(ComputeCellId),
}

#[derive(Debug, PartialEq, Eq)]
pub enum RemoveCallbackError {
    NonexistentCell,
    NonexistentCallback,
}

type ComputeCell<'a, T> = (
    Vec<CellId>,
    Box<dyn Fn(&[T]) -> T + 'a>,
    HashMap<usize, Box<dyn FnMut(T) + 'a>>,
);

pub struct Reactor<'a, T> {
    input_cells: Vec<T>,
    compute_cells: Vec<ComputeCell<'a, T>>,
}

// You are guaranteed that Reactor will only be tested against types that are Copy + PartialEq.
impl<T: Copy + PartialEq> Default for Reactor<'_, T> {
    fn default() -> Self {
        Self::new()
    }
}

impl<'a, T: Copy + PartialEq> Reactor<'a, T> {
    pub fn new() -> Self {
        Self {
            input_cells: Vec::new(),
            compute_cells: Vec::new(),
        }
    }

    // Creates an input cell with the specified initial value, returning its ID.
    pub fn create_input(&mut self, _initial: T) -> InputCellId {
        self.input_cells.push(_initial);

        InputCellId(self.input_cells.len() - 1)
    }

    // Creates a compute cell with the specified dependencies and compute function.
    // The compute function is expected to take in its arguments in the same order as specified in
    // `dependencies`.
    // You do not need to reject compute functions that expect more arguments than there are
    // dependencies (how would you check for this, anyway?).
    //
    // If any dependency doesn't exist, returns an Err with that nonexistent dependency.
    // (If multiple dependencies do not exist, exactly which one is returned is not defined and
    // will not be tested)
    //
    // Notice that there is no way to *remove* a cell.
    // This means that you may assume, without checking, that if the dependencies exist at creation
    // time they will continue to exist as long as the Reactor exists.
    pub fn create_compute<F: Fn(&[T]) -> T + 'a>(
        &mut self,
        _dependencies: &[CellId],
        _compute_func: F,
    ) -> Result<ComputeCellId, CellId> {
        for i in _dependencies {
            match i {
                CellId::Input(InputCellId(input)) => {
                    if *input >= self.input_cells.len() {
                        return Err(CellId::Input(InputCellId(*input)));
                    }
                }
                CellId::Compute(ComputeCellId(compute)) => {
                    if *compute >= self.compute_cells.len() {
                        return Err(CellId::Compute(ComputeCellId(*compute)));
                    }
                }
            }
        }

        self.compute_cells.push((
            _dependencies.to_vec(),
            Box::new(_compute_func),
            HashMap::new(),
        ));

        Ok(ComputeCellId(self.compute_cells.len() - 1))
    }

    // Retrieves the current value of the cell, or None if the cell does not exist.
    //
    // You may wonder whether it is possible to implement `get(&self, id: CellId) -> Option<&Cell>`
    // and have a `value(&self)` method on `Cell`.
    //
    // It turns out this introduces a significant amount of extra complexity to this exercise.
    // We chose not to cover this here, since this exercise is probably enough work as-is.
    pub fn value(&self, id: CellId) -> Option<T> {
        match id {
            CellId::Input(InputCellId(input)) => {
                if input < self.input_cells.len() {
                    Some(self.input_cells[input])
                } else {
                    None
                }
            }
            CellId::Compute(ComputeCellId(compute)) => {
                if compute < self.compute_cells.len() {
                    let (dependencies, compute_func, _callback_func) = &self.compute_cells[compute];
                    let values: Vec<T> = dependencies
                        .iter()
                        .map(|id| self.value(*id).unwrap())
                        .collect();
                    let result = compute_func(values.as_slice());
                    Some(result)
                } else {
                    None
                }
            }
        }
    }

    // Sets the value of the specified input cell.
    //
    // Returns false if the cell does not exist.
    //
    // Similarly, you may wonder about `get_mut(&mut self, id: CellId) -> Option<&mut Cell>`, with
    // a `set_value(&mut self, new_value: T)` method on `Cell`.
    //
    // As before, that turned out to add too much extra complexity.
    pub fn set_value(&mut self, _id: InputCellId, _new_value: T) -> bool {
        fn notify_compute_cells<T: Copy + PartialEq>(
            reactor: &mut Reactor<'_, T>,
            values_before_update: Vec<T>,
            values_after_update: Vec<T>,
        ) {
            values_before_update
                .iter()
                .zip(values_after_update)
                .enumerate()
                .filter(|(_, (before, after))| *before != after)
                .for_each(|(cid, (_before, after))| {
                    if let Some((_, _, callback_funcs)) = reactor.compute_cells.get_mut(cid) {
                        for callback in callback_funcs.values_mut() {
                            callback(after);
                        }
                    }
                });
        }

        fn calc_values_for_dependants<T: Copy + PartialEq>(
            reactor: &mut Reactor<'_, T>,
            _id: InputCellId,
        ) -> Vec<T> {
            reactor
                .compute_cells
                .iter()
                .enumerate()
                .map(|(i, (_, _, _))| reactor.value(CellId::Compute(ComputeCellId(i))).unwrap())
                .collect()
        }

        let InputCellId(input) = _id;

        if input < self.input_cells.len() {
            let values_before_update = calc_values_for_dependants(self, _id);

            self.input_cells[input] = _new_value;

            let values_after_update = calc_values_for_dependants(self, _id);

            notify_compute_cells(self, values_before_update, values_after_update);

            return true;
        }

        false
    }

    // Adds a callback to the specified compute cell.
    //
    // Returns the ID of the just-added callback, or None if the cell doesn't exist.
    //
    // Callbacks on input cells will not be tested.
    //
    // The semantics of callbacks (as will be tested):
    // For a single set_value call, each compute cell's callbacks should each be called:
    // * Zero times if the compute cell's value did not change as a result of the set_value call.
    // * Exactly once if the compute cell's value changed as a result of the set_value call.
    //   The value passed to the callback should be the final value of the compute cell after the
    //   set_value call.
    pub fn add_callback<F: FnMut(T) + 'a>(
        &mut self,
        _id: ComputeCellId,
        _callback: F,
    ) -> Option<CallbackId>
    where
        T: Copy,
    {
        static COUNTER: AtomicUsize = AtomicUsize::new(1);
        match _id {
            ComputeCellId(compute) => {
                if compute >= self.compute_cells.len() {
                    None
                } else {
                    let callback = _callback;
                    if let Some((_, _, callback_funcs)) = self.compute_cells.get_mut(compute) {
                        let counter = COUNTER.fetch_add(1, std::sync::atomic::Ordering::SeqCst);
                        callback_funcs.insert(counter, Box::new(callback));
                        Some(CallbackId(counter))
                    } else {
                        None
                    }
                }
            }
        }
    }

    // Removes the specified callback, using an ID returned from add_callback.
    //
    // Returns an Err if either the cell or callback does not exist.
    //
    // A removed callback should no longer be called.
    pub fn remove_callback(
        &mut self,
        cell: ComputeCellId,
        callback: CallbackId,
    ) -> Result<(), RemoveCallbackError> {
        match cell {
            ComputeCellId(id) => {
                if id >= self.compute_cells.len() {
                    Err(RemoveCallbackError::NonexistentCell)
                } else {
                    let (_compute_cell_id, _, callback_funcs) = &mut self.compute_cells[id];

                    let res = callback_funcs.remove(&callback.0);
                    if res.is_none() {
                        return Err(RemoveCallbackError::NonexistentCallback);
                    }

                    Ok(())
                }
            }
        }
    }
}
