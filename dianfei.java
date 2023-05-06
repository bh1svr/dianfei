import java.security.SecureRandom;
import java.text.DecimalFormat;

public class dianfei{
    public static void main(String []args) {
    	try {
            SecureRandom random = SecureRandom.getInstanceStrong();
            if (args.length > 0) {
        	   System.out.println(random.nextInt(Integer.parseInt(args[0])));
            }
            else {
            	DecimalFormat df = new DecimalFormat("#0.00");
                System.out.println(df.format(random.nextFloat() * 100));
            }
        }
        catch (Exception e) {
        }
    }
}
