
use std::thread;
use std::sync::Arc;

fn main() {
    let top = 100 * 1000 * 1000;
    let xs : Arc<Vec<i64>> = Arc::new((0..top).collect());

    let each = top / 10;
    let mut kids = vec![];
    for ii in 0..10 {
        let local_xs = xs.clone();
        let local_each = each.clone();

        kids.push(thread::spawn(move || {
            let i0 = local_each * ii;
            let i1 = i0 + local_each;

            let mut s = 0;
            for jj in i0..i1 {
                s += local_xs[jj as usize];
            }

            s
        }));
    }

    let mut sum = 0;
    for tt in kids {
        sum += tt.join().unwrap();
    }

    //let y = xs.iter().fold(0, |acc, x| acc + x);
    println!("sum = {}", sum);
}
