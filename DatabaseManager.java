import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Vector;

public class DatabaseManager {
	private static String className = "org.mariadb.jdbc.Driver";
	private static String url = "jdbc:mariadb://127.0.0.1:3306/monitorDB";
	private static String ID = "root";
	private static String PW = "system";
	
	static {
		try {
			Class.forName(className);
		}catch(Exception e) {
			e.printStackTrace();
		}
	}
	
	public Connection getConn() {
		Connection con = null;
		
		try {
			Class.forName(className);
			con = DriverManager.getConnection(url, ID, PW);
		}catch(Exception e) {
			e.printStackTrace();
		}
		
		return con;
	}
	
	public Vector getMonitor() {
		Vector data = new Vector();
		Connection con = null;
		PreparedStatement ps = null;
		ResultSet rs = null;
		
		try {
			con = getConn();
			String sql = new String();
			sql ="SELECT * FROM monitorDB";
			ps = con.prepareStatement(sql);
			rs = ps.executeQuery();
			
			while(rs.next()) {
				String name = rs.getString("mName");
				String prod = rs.getString("mProfessionald");
				int inch = rs.getInt("mInch");
				String panel = rs.getString("mPanel");
				int width = rs.getInt("mWidth");
				int height = rs.getInt("mHeight");
				int price = rs.getInt("mPrice");
				String date = rs.getString("mDate");
				
				Vector row = new Vector();
				row.add(name);
				row.add(prod);
				row.add(inch);
				row.add(panel);
				row.add(width);
				row.add(height);
				row.add(price);
				row.add(date);
				data.add(row);
			}
		} catch(Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if(rs != null) rs.close();
				if(ps != null) ps.close();
				if(con != null) con.close();
			} catch(SQLException e) {
				e.printStackTrace();
			}
		}
		return data;
	}
	
	public Vector searchMonitor(String query) {
		Vector data = new Vector();
		Connection con = null;
		Statement stmt = null;
		ResultSet rs = null;
		
		try {
			con = getConn();
			String sql = new String();
			sql = query;
			stmt = con.createStatement();
			rs = stmt.executeQuery(sql);
			
			
			while(rs.next()) {
				String name = rs.getString("mName");
				String prod = rs.getString("mProfessionald");
				int inch = rs.getInt("mInch");
				String panel = rs.getString("mPanel");
				int width = rs.getInt("mWidth");
				int height = rs.getInt("mHeight");
				int price = rs.getInt("mPrice");
				String date = rs.getString("mDate");
				
				Vector row = new Vector();
				row.add(name);
				row.add(prod);
				row.add(inch);
				row.add(panel);
				row.add(width);
				row.add(height);
				row.add(price);
				row.add(date);
				data.add(row);
			}
		} catch(Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if(rs != null) rs.close();
				if(stmt != null) stmt.close();
				if(con != null) con.close();
			} catch(SQLException e) {
				e.printStackTrace();
			}
		}
		return data;
	}
}
