import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Properties;
import java.util.Scanner;
import java.util.Vector;

@SuppressWarnings("unused")
public class server {
	// MEMBER VARIABLES
	// private Vector<Users> users;
	
	// CLASS METHODS
	// constructor
	public server(String port) {
		System.out.println("Entered the constructor");
		ServerSocket ss = null;
		// !!!!!!!!!!! serverThreads = new Vector<ServerThread>();
		try {
			System.out.println("Trying to bind to port " + port);
			ss = new ServerSocket(Integer.parseInt(port));
			// have right permissions for the port 
			// no other application is using this port 
			System.out.println("Bound to port " + port);
			while(true) {
				Socket s = ss.accept();
				System.out.println("Connection from: " + s.getInetAddress());
				// get one server thread for each client that connnects to the server
				// will start each thread in the ServerThreads constructor 
				// !!!!!!!!!!! ServerThread st = new ServerThread(s, this);
				// adds ServerThread to vector
				// !!!!!!!!!!! serverThreads.add(st);
			}
			
		} catch (IOException ioe) {
			System.out.println("ioe in constructor #1: " + ioe.getMessage());
		} finally {
			try {
				if (ss != null) {
					ss.close();
				}
			} catch (IOException ioe) {
				System.out.println("ioe in constructor #2: " + ioe.getMessage());
			}
		}
	}
	// returns: params read from correctly formed input file
	private static String[] readConfig() {
		String[] params = new String[6];
		
		// initialize I/O variables
		String filename = "";
		boolean validInput = false;
		Scanner sc = new Scanner(System.in);
		
		String ServerHostname, ServerPort, DBConnection, DBUsername, DBPassword, SecretWordFile;
		ServerHostname = ServerPort = DBConnection = DBUsername = DBPassword = SecretWordFile = "";
		
		// loops until input is valid
		while (!validInput) {
			// gets input file from user
			System.out.print("What is the name of the input file? ");
			filename = sc.nextLine();
			try {
				// required params: ServerPort, DBConnection, DBUsername, DBPassword, SecretWordFile
				boolean allParams = true;
				Properties prop = new Properties();
				prop.load(new FileInputStream(filename));
				System.out.println();
				if (prop.getProperty("ServerHostname") != null)	ServerHostname = prop.getProperty("ServerHostname");
				if (prop.getProperty("ServerPort") != null)	ServerPort = prop.getProperty("ServerPort");
				else {
					System.out.println("ServerPort is a required parameter in the configuration file.");
					allParams = false;
				}
				if (prop.getProperty("DBConnection") != null) DBConnection = prop.getProperty("DBConnection");
				else {
					System.out.println("DBConnection is a required parameter in the configuration file.");
					allParams = false;
				}
				if (prop.getProperty("DBUsername") != null) DBUsername = prop.getProperty("DBUsername");
				else {
					System.out.println("DBUsername is a required parameter in the configuration file.");
					allParams = false;
				}
				if (prop.getProperty("DBPassword") != null)	DBPassword = prop.getProperty("DBPassword");
				else {
					System.out.println("DBPassword is a required parameter in the configuration file.");
					allParams = false;
				}
				if (prop.getProperty("SecretWordFile") != null) SecretWordFile = prop.getProperty("SecretWordFile");
				else {
					System.out.println("SecretWordFile is a required parameter in the configuration file.");
					allParams = false;
				}
				// if not all parameters are present, need different config file
				if (!allParams) {
					System.out.println();
					continue;
				}
			} catch (FileNotFoundException fnfe) {
				 System.out.println();
				 System.out.println("Input file does not exist.");
				 System.out.println();
				 continue;
			} catch (IOException ioe) {
				System.out.println();
				System.out.println("Can't read from input file");
				System.out.println();
				continue;
			}
			validInput = true;
			sc.close();
		}
		params[0] = ServerHostname;
		params[1] = ServerPort;
		params[2] = DBConnection;
		params[3] = DBUsername;
		params[4] = DBPassword;
		params[5] = SecretWordFile;
		return params;
	}
	// prints parameters provided by the config file
	private static void printParams(String[] params) {
		for (int i=0; i<params.length; i++) {
			if (i==0) {
				if (params[i].equals("")) continue;
				else System.out.println("ServerHostname - " + params[0]);
			}
			else if (i==1) System.out.println("ServerPort - " + params[1]);
			else if (i==2) System.out.println("DBConnection - " + params[2]);
			else if (i==3) System.out.println("DBUsername - " + params[3]);
			else if (i==4) System.out.println("DBPassword - " + params[4]);
			else if (i==5) System.out.println("SecretWordFile - " + params[5]);
		}
		System.out.println();
	}
	// attempts to connect to database with provided parameters
	private static Connection dbConn(String[] params) {
		System.out.print("Trying to connect to the database...");
		Connection conn = null;
		try {
			conn = DriverManager.getConnection(params[2], params[3], params[4]);
			System.out.println("Connected!");
		} catch (SQLException e) {
			System.out.println("Unable to connect to database " + params[2] + " with username " + params[3] + " and password " + params[4] + ".");
		}
		return conn;
	}
	
	// MAIN METHOD
	public static void main(String[] args) {
		// gets parameters from config file
		String[] params = readConfig();
		// prints parameters
		printParams(params);
		// attempts to connect to db
		@SuppressWarnings("unused")
		Connection conn = dbConn(params);
		// (failed to connect to db)
		//if (conn == null) return;
		// new server with port from config file
		new server(params[1]);
	}
}
