import java.util.*;
public class Main {
    class Climber {
        public int back;
        public int r1, r2, r3;
        int prod() {
            return this.r1 * this.r2 * this.r3;
        }
        int sum() {
            return this.r1 + this.r2 + this.r3;
        }
    }
    public boolean less(Climber x, Climber y) {
        return x.prod() < y.prod() ||
            (x.prod() == y.prod() && x.sum() < y.sum()) ||
            (x.prod() == y.prod() && x.sum() == y.sum() && x.back < y.back);
    }
    public void solve() {
        Scanner sc = new Scanner(System.in);
		int n;
		n = sc.nextInt();
        List<Climber> a = new ArrayList<>();
        int i;
        for(i=0;i<n;i++) {
            Climber x = new Climber();
            x.back = sc.nextInt();
            x.r1 = sc.nextInt();
            x.r2 = sc.nextInt();
            x.r3 = sc.nextInt();
            a.add(x);
        }
        int trial;
        for(trial=0;trial<3;trial++) {
            Climber top = a.get(0);
            for (Climber x : a) {
                if (less(x, top)) {
                    top = x;
                }
            }
            System.out.printf("%d ", top.back);
            a.remove(top);
        }
        System.out.printf("\n");
    }
	public static void main(String args[]){
        Main s = new Main();
        s.solve();
	}
}