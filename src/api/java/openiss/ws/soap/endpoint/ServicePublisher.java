package openiss.ws.soap.endpoint;

import openiss.ws.soap.service.OpenISSSOAPService;
import openiss.ws.soap.service.OpenISSSOAPServiceImpl;

import javax.xml.ws.Endpoint;
import java.io.File;
import java.util.concurrent.TimeUnit;


public class ServicePublisher {

    static int port = 9090;
    static String service = "openiss";
    static String url = "http://localhost:" + port + "/" + service;
    static String FAKENECT_PATH = System.getenv("FAKENECT_PATH");

    public static void main(String[] args) throws Exception {
        System.out.println("SOAP Service listening on " + url + "?wsdl");

        Endpoint.publish(url, new OpenISSSOAPServiceImpl());

        OpenISSSOAPServiceImpl object = new OpenISSSOAPServiceImpl();

        File dir = new File(FAKENECT_PATH);
        File[] directoryListing = dir.listFiles();
        if (directoryListing != null) {
            while(true){
                for (File child : directoryListing) {

                    if(child.getName().endsWith(".ppm")){
                        TimeUnit.SECONDS.sleep(1);
                        object.setFileName(child.getName());
                    }
                }
            }
        } else {
            System.out.println("Error: no such directory");
        }
    }
}