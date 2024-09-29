 /*
  *   Using JMS, create a system for testing and grading. The system should contain:
  *    - a function Start(bool teacher, string courseID) which, for the teacher (true), enables receiving responses from students, and for the student (false), allows receiving a question sent by the teacher. 
  *    - There can be multiple students and teachers per course code. 
  *    - When grading, ensure that only one teacher performs the grading. Then, the system also contains:
  *        - a function SendQuestion(string courseID, string questionID, string question) through which the teacher sends a question to the students, where the question is in text form. 
  *    
  *    - Upon receipt, the system displays the question on the student's screen. 
  *   - Additionally, the system contains a function for sending responses. 
  *    - The response is displayed to the teacher who is grading. 
  *    - It is not necessary to implement the calls to these functions, only the functions themselves. 
  *    - Consider the rest of the system as implemented. 
  *    - Ensure that only interested parties receive the messages. At the beginning, sketch out the flow of message exchange.
  */

 /*
  * Messages being sent are: (topic) QUESTION - sent to multiple, and (queue) ANSWER - sent to one
  */

import javax.naming.InitialContext;
import javax.naming.NamingException;

public class System {

    private int studentID;

    private Topic topicQuestion;
    private Queue queueAnswer;

    private TopicConnection tc;
    private TopicSession ts;

    private QueueConnection qc;
    private QueueSession qs;

    private TopicPublisher topicQuestionPub;
    private TopicSubscriber topicQuestionSub;

    private QueueSender queueAnswerSend;
    private QueueReceiver queueAnswerRecv;

    public System(int studentID) throws NamingException, JMSException {
    
        this.studentID = studentID;

        InitialContext ictx = new InitialContext();
        TopicConnectionFactory tcf = (TopicConnectionFactory) ictx.lookup("tcf");
        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qcf");

        this.topicQuestion = (Topic) ictx.lookup("topicQuestion");
        this.queueAnswer = (Topic) ictx. lookup("queueAnswer");

        ictx.close();

        this.tc = tcf.createTopicConnection();
        this.ts = this.tc.createTopicSession(false, Session.AUTO_ACKNOWLEDGE);

        this.qc = qcf.createQueueConnection();
        this.qs = this.tc.createQueueSession(false, Session.AUTO_ACKNOWLEDGE);

        this.topicQuestionPub = this.ts.createPublisher(this.topicQuestion);
        this.queueAnswerSend = this.qs.createSender(this.queueAnswer);
	
	this.tc.start();
	this.qc.start();

    }

    public void Start(bool teacher, string courseID)
    {
        if (teacher)
        {
            this.queueAnswerRecv = this.ts.createReceiver(this.queueAnswer, "CourseID LIKE '%" + courseID + "'");
            this.queueAnswerRecv.setMessageListener( new AnswerMessageListener() );
        }
        else {
            this.topicQuestionSub = this.ts.createSubscriber(this.topicQuestion, "CourseID LIKE '%" + courseID + "'");
            this.topicQuestionSub.setMessageListener( new QuestionMessageListener() );
        }
    }

    public void SendQuestion(string courseID, string questionID, string question)
    {
        TextMessage m = ts.createTextMessage();

        m.setStringProperty("CourseID", courseID);
        m.setStringProperty("QuestionID", questionID);
        m.setText(question);

        topicQuestionPub.send(m);
    }

    public void SendAnswer(string courseID, string questionID, string answer)
    {
        TextMessage m = ts.createTextMessage();

        m.setIntProperty("StudentID", this.studentID);
        m.setStringProperty("CourseID", courseID);
        m.setStringProperty("QuestionID", questionID);
        m.setText(answer);

        topicAnswerPub.send(m);
    }
  }

  public class AnswerMessageListener implements MessageListener {
    
    public AnswerMessageListener() {}

    @Override
    public void onMessage(Message m)
    {
        TextMessage tm = (TextMessage) m;
        string answer = tm.getText();

        ing studentID = tm.getIntProperty("StudentID");
        string courseID = tm.getStringProperty("CourseID");
        string questionID = tm.getStringProperty("QuestionID");
        
        System.out.println("Student with id " + studentID + " answered question " + questionID + " from course " + courseID + ": " + answer);
    }
  }

  public class QuestionMessageListener implements MessageListener {
    
    public QuestionMessageListener() {}

    @Override
    public void onMessage(Message m)
    {
        TextMessage tm = (TextMessage) m;
        string question = tm.getText();

        string courseID = tm.getStringProperty("CourseID");
        string questionID = tm.getStringProperty("QuestionID");

        System.out.printl("Question with id " + questionID + " from course " + courseID + " is: " + question);
    }
  }