import java.awt.FlowLayout;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JCheckBox;
import javax.swing.JComponent;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.ListSelectionModel;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;

@SuppressWarnings("serial")
class MonitorFrame extends JFrame implements ActionListener{
	DatabaseManager databaseManager;
	Monitor monitor;
	
	JLabel lbProd, lbInch, lbPanel, lbWidth, lbHeight;
	JCheckBox[] cbProd = new JCheckBox[8];
	JCheckBox[] cbInch = new JCheckBox[8];
	JCheckBox[] cbPanel = new JCheckBox[5];
	JCheckBox[] cbWidth = new JCheckBox[7];
	JCheckBox[] cbHeight = new JCheckBox[6];
	ButtonGroup group1 = new ButtonGroup(), group2 = new ButtonGroup();
	JRadioButton rb1, rb2, rb3, rb4;
	JButton btnRefresh, btnClear;
	JTable table;
	JScrollPane pane;
	JTextField tfSearch, tfLow, tfHigh;
	Vector<String> col;
	String query;
	String queryProd, queryInch, queryPanel, queryWidth, queryHeight, querySort, querySearch, queryRange, queryLow, queryHigh;
	
	GridBagLayout gb;
	GridBagConstraints gbc;
	
	public MonitorFrame(){
		databaseManager = new DatabaseManager();
		gb = new GridBagLayout();
		setLayout(gb);
		gbc = new GridBagConstraints();
		gbc.fill = GridBagConstraints.BOTH;
		gbc.weightx = 1.0;
		gbc.weighty = 1.0;
		
		//JCheckBox[] cbProd = new JCheckBox[8];
		JPanel pProd = new JPanel();
		
		cbProd[0] = new JCheckBox("삼성");
		cbProd[1] = new JCheckBox("LG");
		cbProd[2] = new JCheckBox("DELL");
		cbProd[3] = new JCheckBox("BenQ");
		cbProd[4] = new JCheckBox("AlphaScan");
		cbProd[5] = new JCheckBox("CrossOver");
		cbProd[6] = new JCheckBox("한성");
		cbProd[7] = new JCheckBox("HP");
		
		pProd.setBorder(BorderFactory.createTitledBorder("제조사"));
		pProd.setLayout(new FlowLayout(FlowLayout.LEFT));
		for(int i=0; i<8; i++) pProd.add(cbProd[i]);
		
		gbAdd(pProd, 0, 0, 1, 1);
		
		JPanel pInch = new JPanel();
		
		cbInch[0] = new JCheckBox("22");
		cbInch[1] = new JCheckBox("23");
		cbInch[2] = new JCheckBox("24");
		cbInch[3] = new JCheckBox("25");
		cbInch[4] = new JCheckBox("27");
		cbInch[5] = new JCheckBox("28");
		cbInch[6] = new JCheckBox("32");
		cbInch[7] = new JCheckBox("34");
		
		pInch.setBorder(BorderFactory.createTitledBorder("인치"));
		pInch.setLayout(new FlowLayout(FlowLayout.LEFT));
		for(int i=0; i<8; i++) pInch.add(cbInch[i]);
		
		gbAdd(pInch, 0, 1, 1, 1);
		
		JPanel pPanel = new JPanel();
		
		cbPanel[0] = new JCheckBox("IPS");
		cbPanel[1] = new JCheckBox("VA");
		cbPanel[2] = new JCheckBox("TN");
		cbPanel[3] = new JCheckBox("PLS");
		cbPanel[4] = new JCheckBox("AHVA");
		
		pPanel.setBorder(BorderFactory.createTitledBorder("패널"));
		pPanel.setLayout(new FlowLayout(FlowLayout.LEFT));
		for(int i=0; i<5; i++) pPanel.add(cbPanel[i]);
		
		gbAdd(pPanel, 0, 2, 1, 1);
		
		JPanel pWidth = new JPanel();
		
		cbWidth[0] = new JCheckBox("1920");
		cbWidth[1] = new JCheckBox("2048");
		cbWidth[2] = new JCheckBox("2560");
		cbWidth[3] = new JCheckBox("3440");
		cbWidth[4] = new JCheckBox("3840");
		cbWidth[5] = new JCheckBox("4096");
		cbWidth[6] = new JCheckBox("5120");
		
		pWidth.setBorder(BorderFactory.createTitledBorder("가로해상도"));
		pWidth.setLayout(new FlowLayout(FlowLayout.LEFT));
		for(int i=0; i<7; i++) pWidth.add(cbWidth[i]);
		
		gbAdd(pWidth, 0, 3, 1, 1);
		
		JPanel pHeight = new JPanel();
		
		cbHeight[0] = new JCheckBox("1080");
		cbHeight[1] = new JCheckBox("1200");
		cbHeight[2] = new JCheckBox("1440");
		cbHeight[3] = new JCheckBox("1600");
		cbHeight[4] = new JCheckBox("2160");
		cbHeight[5] = new JCheckBox("2880");
		
		pHeight.setBorder(BorderFactory.createTitledBorder("세로해상도"));
		pHeight.setLayout(new FlowLayout(FlowLayout.LEFT));
		for(int i=0; i<6; i++) pHeight.add(cbHeight[i]);
		
		gbAdd(pHeight, 0, 4, 1, 1);
		
		JPanel pSort = new JPanel();
		
		rb1 = new JRadioButton("가격 높은 순");
		rb2 = new JRadioButton("가격 낮은 순");
		rb3 = new JRadioButton("등록일 최신 순");
		rb4 = new JRadioButton("등록일 오래된 순");
		btnClear = new JButton("초기화");
		btnClear.addActionListener(this);
		
		group1.add(rb1); group1.add(rb2); group2.add(rb3); group2.add(rb4);
		pSort.setBorder(BorderFactory.createTitledBorder("정렬 기준"));
		pSort.setLayout(new FlowLayout(FlowLayout.LEFT));
		pSort.add(rb1); pSort.add(rb2); pSort.add(rb3); pSort.add(rb4); pSort.add(btnClear);
		
		gbAdd(pSort, 0, 5, 1, 1);
		
		JPanel pSearch = new JPanel();
		JLabel lbSearch = new JLabel("제품 이름으로 검색");
		JLabel lbBound = new JLabel("가격 범위로 검색");
		JLabel lbBetween = new JLabel("~");
		
		tfSearch = new JTextField("", 20);
		tfSearch.setEditable(true);
		tfSearch.setBounds(0, 0, 80, 50);
		tfSearch.setHorizontalAlignment(JTextField.LEFT);
		tfLow = new JTextField("", 10);
		tfLow.setEditable(true);
		tfHigh = new JTextField("", 10);
		tfHigh.setEditable(true);
		btnRefresh = new JButton("검색");
		btnRefresh.addActionListener(this);
		
		pSearch.add(lbSearch);
		pSearch.add(tfSearch);
		pSearch.add(lbBound);
		pSearch.add(tfLow);
		pSearch.add(lbBetween);
		pSearch.add(tfHigh);
		pSearch.add(btnRefresh);
		pSearch.setBorder(BorderFactory.createTitledBorder("검색"));

		gbAdd(pSearch, 0, 6, 1, 1);
		
		col = new Vector<String>();
		col.add("제품명");
		col.add("제조사");
		col.add("인치");
		col.add("패널");
		col.add("너비");
		col.add("높이");
		col.add("가격");
		col.add("출시일자");
		
		DefaultTableModel model = new DefaultTableModel(databaseManager.getMonitor(), col) {
			public boolean isCellEditable(int row, int column) {
				return false;
			}
		};
		
		table = new JTable(model);
		JScrollPane scroll = new JScrollPane(table);
		jTableSet();

		gbAdd(scroll, 0, 7, 1, 6);
		
		
		setSize(600, 800);
		setTitle("모니터 검색 시스템");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		pack();
		setVisible(true);
	}
	
	private void gbAdd(JComponent c, int x, int y, int w, int h) {
		gbc.gridx = x;
		gbc.gridy = y;
		gbc.gridwidth = w;
		gbc.gridheight = h;
		gb.setConstraints(c, gbc);
		gbc.insets = new Insets(2, 2, 2, 2);
		add(c, gbc);
	}
	
	public void jTableSet() {
		table.getTableHeader().setReorderingAllowed(false);;
		table.getTableHeader().setResizingAllowed(false);
		table.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
		
		DefaultTableCellRenderer celAlignCenter = new DefaultTableCellRenderer();
		celAlignCenter.setHorizontalAlignment(JLabel.CENTER);
		
		for(int i=0; i<table.getColumnCount(); i++) {
			table.getColumnModel().getColumn(i).setPreferredWidth(10);
			table.getColumnModel().getColumn(i).setCellRenderer(celAlignCenter);
		}
	}
	
	public void jTableRefresh() {
		genQuery();
		DefaultTableModel model = new DefaultTableModel(databaseManager.searchMonitor(query), col) {
			public boolean isCellEditable(int row, int column) {
				return false;
			}
		};
		table.setModel(model);
		jTableSet();
	}
	
	public void genQuery(){
		int cbChecked = 0, cbCheckProd = 0 , cbCheckInch = 0, cbCheckPanel = 0, cbCheckWidth = 0, cbCheckHeight = 0;
		query = new String();
		queryProd = new String();
		queryInch = new String();
		queryPanel = new String();
		queryWidth = new String();
		queryHeight = new String();
		querySort = new String(); 
		querySearch = new String(); 
		queryLow = new String();
		queryHigh = new String();
		queryRange = new String();
		
		query = "SELECT * FROM MonitorDB";
		
		//Prod 갯수 세기
		for(int i=0; i<8; i++) {
			if(cbProd[i].isSelected() == true) {
				cbCheckProd++;
				cbChecked++;
			}
		};
		for(int i=0; i<8; i++) {
			if(cbCheckProd == 0) {
				queryProd = " ";
			}
			else if (cbCheckProd == 1) {
				for(int j=0; j<8; j++) {
					if(cbProd[j].isSelected() == true) queryProd = " mProfessionald = \"" + cbProd[j].getText() + "\"";
				}
			}
			else {
				int tempProd = 0;
				queryProd = " (";
				for(int j=0; j<8; j++) {
					if(cbProd[j].isSelected() == true) {
						queryProd = queryProd.concat("mProfessionald = \"" + cbProd[j].getText() + "\"");
						tempProd = j;
						break;
					}
				}
				for(int j=tempProd + 1; j<8; j++) {
					if(cbProd[j].isSelected() == true) {
						queryProd = queryProd.concat(" OR mProfessionald = \"" + cbProd[j].getText() + "\"");
					}
				}
				queryProd = queryProd.concat(")");
			}
		}
		
		//Inch 갯수 세기
		for(int i=0; i<8; i++) {
			if(cbInch[i].isSelected() == true) {
				cbCheckInch++;
				cbChecked++;
			}
		};
		for(int i=0; i<8; i++) {
			if(cbCheckInch == 0) {
				queryInch = " ";
			}
			else if (cbCheckInch == 1) {
				if (cbCheckProd != 0) queryInch = queryInch.concat(" AND");
				for(int j=0; j<8; j++) {
					if(cbInch[j].isSelected() == true ) queryInch = queryInch.concat(" mInch = \"" + cbInch[j].getText() + "\"");
				}
				break;
			}
			else {
				int tempInch = 0;
				if (cbCheckProd != 0) queryInch = " AND (";
				else queryInch = " (";
				for(int j=0; j<8; j++) {
					if(cbInch[j].isSelected() == true) {
						queryInch = queryInch.concat("mInch = \"" + cbInch[j].getText() + "\"");
						tempInch = j;
						break;
					}
				}
				for(int j=tempInch + 1; j<8; j++) {
					if(cbInch[j].isSelected() == true) {
						queryInch = queryInch.concat(" OR mInch = \"" + cbInch[j].getText() + "\"");
					}
				}
				queryInch = queryInch.concat(")");
			}
		}
		
		//Panel 갯수 세기
		for(int i=0; i<5; i++) {
			if(cbPanel[i].isSelected() == true) {
				cbCheckPanel++;
				cbChecked++;
			}
		};
		for(int i=0; i<5; i++) {
			if(cbCheckPanel == 0) {
				queryPanel = " ";
			}
			else if (cbCheckPanel == 1) {
				if (cbCheckProd != 0 || cbCheckInch != 0) queryPanel = queryPanel.concat(" AND");
				for(int j=0; j<5; j++) {
					if(cbPanel[j].isSelected() == true) queryPanel = queryPanel.concat(" mPanel = \"" + cbPanel[j].getText() + "\"");
				}
				break;
			}
			else {
				int tempPanel = 0;
				if (cbCheckProd != 0 || cbCheckInch != 0) queryPanel = " AND (";
				else queryPanel = " (";
				for(int j=0; j<5; j++) {
					if(cbPanel[j].isSelected() == true) {
						queryPanel = queryPanel.concat("mPanel = \"" + cbPanel[j].getText() + "\"");
						tempPanel = j;
						break;
					}
				}
				for(int j=tempPanel + 1; j<5; j++) {
					if(cbPanel[j].isSelected() == true) {
						queryPanel = queryPanel.concat(" OR mPanel = \"" + cbPanel[j].getText() + "\"");
					}
				}
				queryPanel = queryPanel.concat(")");
			}
		}
		
		//Width 갯수 세기
		for(int i=0; i<7; i++) {
			if(cbWidth[i].isSelected() == true) {
				cbCheckWidth++;
				cbChecked++;
			}
		};
		for(int i=0; i<7; i++) {
			if(cbCheckWidth == 0) {
				queryWidth = " ";
			}
			else if (cbCheckWidth == 1) {
				if(cbCheckProd != 0 || cbCheckInch != 0 || cbCheckPanel != 0) queryWidth = queryWidth.concat(" AND");
				for(int j=0; j<7; j++) {
					if(cbWidth[j].isSelected() == true) queryWidth = queryWidth.concat(" mWidth = \"" + cbWidth[j].getText() + "\"");
				}
				break;
			}
			else {
				int tempWidth = 0;
				if(cbCheckProd != 0 || cbCheckInch != 0 || cbCheckPanel != 0) queryWidth = " AND (";
				else queryWidth = " (";
				for(int j=0; j<7; j++) {
					if(cbWidth[j].isSelected() == true) {
						queryWidth = queryWidth.concat("mWidth = \"" + cbWidth[j].getText() + "\"");
						tempWidth = j;
						break;
					}
				}
				for(int j=tempWidth + 1; j<7; j++) {
					if(cbWidth[j].isSelected() == true) {
						queryWidth = queryWidth.concat(" OR mWidth = \"" + cbWidth[j].getText() + "\"");
					}
				}
				queryWidth = queryWidth.concat(")");
			}
		}
		
		//Height 갯수 세기
		for(int i=0; i<6; i++) {
			if(cbHeight[i].isSelected() == true) {
				cbCheckHeight++;
				cbChecked++;
			}
		};
		for(int i=0; i<6; i++) {
			if(cbCheckHeight == 0) {
				queryHeight = " ";
			}
			else if (cbCheckHeight == 1) {
				if(cbCheckProd != 0 || cbCheckInch != 0 || cbCheckPanel != 0 || cbCheckWidth != 0) queryHeight = queryHeight.concat(" AND");
				for(int j=0; j<6; j++) {
					if(cbHeight[j].isSelected() == true) queryHeight = queryHeight.concat(" mHeight = \"" + cbHeight[j].getText() + "\"");
				}
				break;
			}
			else {
				int tempHeight = 0;
				if(cbCheckProd != 0 || cbCheckInch != 0 || cbCheckPanel != 0 || cbCheckWidth != 0) queryHeight = " AND (";
				else queryHeight = " (";
				for(int j=0; j<6; j++) {
					if(cbHeight[j].isSelected() == true) {
						queryHeight = queryHeight.concat("mHeight = \"" + cbHeight[j].getText() + "\"");
						tempHeight = j;
						break;
					}
				}
				for(int j=tempHeight + 1; j<6; j++) {
					if(cbHeight[j].isSelected() == true) {
						queryHeight = queryHeight.concat(" OR mHeight = \"" + cbHeight[j].getText() + "\"");
					}
				}
				queryHeight = queryHeight.concat(")");
			}
		}
		
		//정렬기준
		querySort = "";
		if(rb1.isSelected()) {
			querySort = " ORDER BY mPrice DESC";
			if(rb3.isSelected()) querySort = querySort.concat(", mDate DESC");
			else if (rb4.isSelected()) querySort = querySort.concat(", mDate ASC");
		} else if (rb2.isSelected()) {
			querySort = " ORDER BY mPrice ASC";
			if(rb3.isSelected()) querySort = querySort.concat(", mDate DESC");
			else if (rb4.isSelected()) querySort = querySort.concat(", mDate ASC");
		} else if (!rb1.isSelected() && !rb2.isSelected()) {
			if(rb3.isSelected()) querySort = " ORDER BY mDate DESC";
			else if (rb4.isSelected()) querySort = " ORDER BY mDate ASC";
		}
		
		//검색창 읽기
		querySearch = "";
		if(!tfSearch.getText().isEmpty()) querySearch = " mName LIKE \"%" + tfSearch.getText() + "%\"";
		
		
		//범위검색 읽기
		queryRange = "";
		if(!tfLow.getText().isEmpty()) queryRange = " mPrice >= " + tfLow.getText();
		else if (!tfHigh.getText().isEmpty()) queryRange = " mPrice <= " + tfHigh.getText();
		if(!tfHigh.getText().isEmpty() && !tfLow.getText().isEmpty()) queryRange = " mPrice >= " + tfLow.getText() + " AND mPrice <= " + tfHigh.getText();
		
		//쿼리 합치기
		if(cbChecked > 0) query = query.concat(" WHERE" + queryProd + queryInch + queryPanel + queryWidth + queryHeight);
		if(cbChecked > 0 && !querySearch.isEmpty()) query = query.concat(" AND" + querySearch);
		else if(!querySearch.isEmpty()) query = query.concat(" WHERE" + querySearch);
		if((cbChecked > 0 || !querySearch.isEmpty()) && !queryRange.isEmpty() ) query = query.concat(" AND" + queryRange);
		else if(!queryRange.isEmpty()) query = query.concat(" WHERE" + queryRange);
		query = query.concat(querySort);
	}
	
	@Override
	public void actionPerformed(ActionEvent e) {
		String ButtonFlag = e.getActionCommand();
		
		if(ButtonFlag.equals("검색")) {
			try {
				jTableRefresh();
			} catch (Exception ee) {
				JOptionPane.showMessageDialog(this, "SQL 전송 오류");
			}
		}
		
		if(ButtonFlag.contentEquals("초기화")) {
			try {
				group1.clearSelection();
				group2.clearSelection();
			} catch (Exception ee) {
				
			}
		}
	}
}
